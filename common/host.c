/*
	host.c
	
	(description)
	
	Copyright (C) 1999,2000  contributors of the QuakeForge project
	Please see the file "AUTHORS" for a list of contributors

	Author: Jeff Teunissen	<deek@quakeforge.net>
	Date:	09 Feb 2000
	
	This file is part of the QuakeForge Core system.
	
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:
	
	Free Software Foundation, Inc.
	59 Temple Place - Suite 330
	Boston, MA  02111-1307, USA.
*/

/*
	Host_EndGame

	On clients and non-dedicated servers, close currently-running game and
	drop to console. On dedicated servers, exit.
*/

#include <stdarg.h>
#include <stdio.h>
#include <setjmp.h>

#include "net.h"
#include "console.h"
#include "quakedef.h"
#ifdef UQUAKE
#include "server.h"
#endif
#include "client.h"
#include "view.h"
#include "wad.h"
#include "input.h"
#include "sound.h"
#include "cdaudio.h"
#include "keys.h"
#include "menu.h"
#include "draw.h"
#include "screen.h"
#include "sbar.h"
#include "mathlib.h"

extern int host_hunklevel;

jmp_buf 	host_abort;

double		realtime;				// without any filtering or bounding
double		oldrealtime;			// last frame run

int			fps_count;
int vcrFile = -1;

void
Host_EndGame ( char *message, ... )
{
	va_list argptr;
	char	string[1024];

	va_start (argptr, message);
	vsnprintf (string, sizeof(string), message, argptr);
	va_end (argptr);
	
	Con_Printf ("\n===========================\n");
	Con_Printf ("Host_EndGame: %s\n", string);
	Con_Printf ("===========================\n\n");
	
#if QUAKEWORLD
	CL_Disconnect ();
	
	longjmp (host_abort, 1);
#elif UQUAKE
	if ( sv.active )
		Host_ShutdownServer (false);

	if ( cls.state == ca_dedicated )
		Sys_Error ("Host_EndGame: %s\n",string);	// dedicated servers exit
	
	if ( cls.demonum != -1 )
		CL_NextDemo ();
	else
		CL_Disconnect ();

	longjmp (host_abortserver, 1);
#endif
}


/*
	Host_FrameMain

	Run everything that happens on a per-frame basis
*/
int		nopacketcount;

void
Host_FrameMain (float time)
{
	static double		time1 = 0;
	static double		time2 = 0;
	static double		time3 = 0;
	int			pass1, pass2, pass3;
	float fps;
	if (setjmp (host_abort) )
		return;			// something bad happened, or the server disconnected

	// decide the simulation time
	realtime += time;
	if (oldrealtime > realtime)
		oldrealtime = 0;

	if (cl_maxfps.value)
		fps = max(30.0, min(cl_maxfps.value, 72.0));
	else
		fps = max(30.0, min(rate.value/80.0, 72.0));

	if (!cls.timedemo && realtime - oldrealtime < 1.0/fps)
		return;			// framerate is too high

	host_frametime = realtime - oldrealtime;
	oldrealtime = realtime;
	if (host_frametime > 0.2)
		host_frametime = 0.2;
		
	// get new key events
	Sys_SendKeyEvents ();

	// allow mice or other external controllers to add commands
	IN_Commands ();

	// process console commands
	Cbuf_Execute ();

	// fetch results from server
	CL_ReadPackets ();

	// send intentions now
	// resend a connection request if necessary
	if (cls.state == ca_disconnected) {
		CL_CheckForResend ();
	} else
		CL_SendCmd ();

	// Set up prediction for other players
	CL_SetUpPlayerPrediction(false);

	// do client side motion prediction
	CL_PredictMove ();

	// Set up prediction for other players
	CL_SetUpPlayerPrediction(true);

	// build a refresh entity list
	CL_EmitEntities ();

	// update video
	if (host_speeds.value)
		time1 = Sys_DoubleTime ();

	SCR_UpdateScreen ();

	if (host_speeds.value)
		time2 = Sys_DoubleTime ();
		
	// update audio
	if (cls.state == ca_active)
	{
		S_Update (r_origin, vpn, vright, vup);
		CL_DecayLights ();
	}
	else
		S_Update (vec3_origin, vec3_origin, vec3_origin, vec3_origin);
	
	CDAudio_Update();

	if (host_speeds.value)
	{
		pass1 = (time1 - time3)*1000;
		time3 = Sys_DoubleTime ();
		pass2 = (time2 - time1)*1000;
		pass3 = (time3 - time2)*1000;
		Con_Printf ("%3i tot %3i server %3i gfx %3i snd\n",
					pass1+pass2+pass3, pass1, pass2, pass3);
	}

	host_framecount++;
	fps_count++;
}


/*
	Host_Frame

	For QW, just call Host_FrameMain; for UQ, check if we want profiling
	information. If we do, time how long it took for the per-frame stuff
	to be handled and write it to console.
*/
void
Host_Frame ( float time )
{
#ifdef QUAKEWORLD
	Host_FrameMain (time);
	return;
#elif UQUAKE
	double			time1, time2;
	static double	timetotal;
	static int		timecount;
	int				i, c, m;

	if (!serverprofile.value) {
		Host_FrameMain (time);
		return;
	}
	
	time1 = Sys_DoubleTime ();
	Host_FrameMain (time);
	time2 = Sys_DoubleTime ();	
	
	timetotal += time2 - time1;
	timecount++;
	
	if (timecount < 1000)
		return;

	m = timetotal*1000/timecount;
	timecount = 0;
	timetotal = 0;
	c = 0;
	for (i=0 ; i < svs.maxclients ; i++) {
		if (svs.clients[i].active)
			c++;
	}

	Con_Printf ("serverprofile: %2i clients %2i msec\n",  c,  m);
#endif
}


/*
	Host_Error

	Shut down client. If server running, shut that down too.
*/
void
Host_Error ( char *error, ... )
{
	va_list 			argptr;
	char				string[1024];
	static qboolean 	inerror = false;
	
	if ( inerror ) {
		Sys_Error ("Host_Error: Called recursively from within an error");
	}
	inerror = true;
	
	va_start (argptr, error);
	vsnprintf (string, sizeof(string), error, argptr);
	va_end (argptr);
	Con_Printf ("Host_Error: %s\n", string);

#ifdef UQUAKE	
	if (sv.active)
		Host_ShutdownServer (false);

	if (cls.state == ca_dedicated)
		Sys_Error ("Host_Error: %s\n",string);	// dedicated servers exit
#endif
	
	CL_Disconnect ();
	cls.demonum = -1;

	inerror = false;

// FIXME
	Sys_Error ("Host_Error: %s\n",string);

#ifdef UQUAKE
	longjmp (host_abortserver, 1);
#endif
}


/*
	Host_InitVCR
	
	Set up playback and recording of demos.
*/
extern int vcrFile;
#define	VCR_SIGNATURE	0x56435231		// "VCR1"

void
Host_InitVCR ( quakeparms_t *parms )
{
	int		i, len, n;
	char	*p;
	
	if ( COM_CheckParm("-playback") ) {
		if ( com_argc != 2 )
			Sys_Error("No other parameters allowed with -playback\n");

		Sys_FileOpenRead("quake.vcr", &vcrFile);
		if ( vcrFile == -1 )
			Sys_Error("playback file not found\n");

		Sys_FileRead (vcrFile, &i, sizeof(int));
		if ( i != VCR_SIGNATURE )
			Sys_Error("Invalid signature in vcr file\n");

		Sys_FileRead (vcrFile, &com_argc, sizeof(int));
		com_argv = malloc(com_argc * sizeof(char *));
		com_argv[0] = parms->argv[0];
		for (i = 0 ; i < com_argc ; i++) {
			Sys_FileRead (vcrFile, &len, sizeof(int));
			p = malloc(len);
			Sys_FileRead (vcrFile, p, len);
			com_argv[i+1] = p;
		}
		com_argc++; /* add one for arg[0] */
		parms->argc = com_argc;
		parms->argv = com_argv;
	}

	n = COM_CheckParm("-record");
	if ( n ) {
		vcrFile = Sys_FileOpenWrite("quake.vcr");

		i = VCR_SIGNATURE;
		Sys_FileWrite(vcrFile, &i, sizeof(int));
		i = com_argc - 1;
		Sys_FileWrite(vcrFile, &i, sizeof(int));
		for (i = 1 ; i < com_argc ; i++) {
			if (i == n) {
				len = 10;
				Sys_FileWrite(vcrFile, &len, sizeof(int));
				Sys_FileWrite(vcrFile, "-playback", len);
				continue;
			}
			len = Q_strlen(com_argv[i]) + 1;
			Sys_FileWrite(vcrFile, &len, sizeof(int));
			Sys_FileWrite(vcrFile, com_argv[i], len);
		}
	}
}


/*
	Host_Init
	
	System Startup
*/
void
Host_Init ( quakeparms_t *parms)
{
	COM_InitArgv (parms->argc, parms->argv);

#ifdef QUAKEWORLD
	COM_AddParm ("-game");
	COM_AddParm ("qw");
	Sys_mkdir("qw");
#endif

	if ( COM_CheckParm ("-minmemory") )
		parms->memsize = MINIMUM_MEMORY;
	
	host_parms = *parms;

	if (parms->memsize < MINIMUM_MEMORY)
		Sys_Error ("Only %4.1fMB of memory reported, can't execute game", parms->memsize / (float) 0x100000);
	
	Memory_Init (parms->membase, parms->memsize);
	Cbuf_Init ();
	Cmd_Init ();
	V_Init ();
	
#ifdef QUAKEWORLD
	COM_Init ();

	NET_Init (PORT_CLIENT);
	Netchan_Init ();
#elif UQUAKE
	Chase_Init ();
	Host_InitVCR (parms);
	
	COM_Init (parms->basedir);
	Host_InitLocal ();
#endif

	W_LoadWadFile ("gfx.wad");
	Key_Init ();
	Con_Init ();	
	M_Init ();	

#ifdef UQUAKE	
	PR_Init ();
#endif
	
	Mod_Init ();

#ifdef UQUAKE
	NET_Init ();	
	SV_Init ();
#endif

	Con_Printf ("Exe: "__TIME__" "__DATE__"\n");
	Con_Printf ("%4.1f megabytes RAM used.\n", (parms->memsize / (1024 * 1024.0)) );
	
	R_InitTextures ();		// needed even for UQ dedicated server

#ifdef UQUAKE
	if (cls.state != ca_dedicated) {
#endif
		host_basepal = (byte *)COM_LoadHunkFile ("gfx/palette.lmp");
		if (!host_basepal)
			Sys_Error ("Couldn't load gfx/palette.lmp");
		host_colormap = (byte *)COM_LoadHunkFile ("gfx/colormap.lmp");
		if (!host_colormap)
			Sys_Error ("Couldn't load gfx/colormap.lmp");

		VID_Init(host_basepal);
		IN_Init();
		Draw_Init();
		SCR_Init();
		R_Init();

#ifdef QUAKEWORLD
		cls.state = ca_disconnected;
#endif

		CDAudio_Init();
		Sbar_Init();
		CL_Init();
#ifdef UQUAKE
	}
#endif

	Cbuf_InsertText ("exec quake.rc\n");

#ifdef QUAKEWORLD	
	Cbuf_AddText ("echo Type connect <internet address> or use GameSpy to connect to a game.\n");
	Cbuf_AddText ("cl_warncmd 1\n");
#endif

	Hunk_AllocName (0, "-HOST_HUNKLEVEL-");
	host_hunklevel = Hunk_LowMark ();
	host_initialized = true;

#ifdef QUAKEWORLD	
	Con_Printf ("\nClient Version %s\n\n", QF_VERSION);
#endif
	Sys_Printf ("======= QuakeForge Initialized =======\n");	
}


/*
	Host_Shutdown

	Callback from Sys_Quit and Sys_Error.  It would be better to run quit
	through here before the final handoff to the sys code.
*/
void
Host_Shutdown( void )
{
	static qboolean 	isdown = false;
	
	if ( isdown ) {
		printf ("recursive shutdown\n");
		return;
	}
	isdown = true;

#if UQUAKE
// keep Con_Printf from trying to update the screen
	scr_disabled_for_loading = true;
#endif

	Host_WriteConfiguration (); 
		
	CDAudio_Shutdown ();
	NET_Shutdown ();
	S_Shutdown();
	IN_Shutdown ();

#if QUAKEWORLD		
	if (host_basepal) {
#elif UQUAKE
	if (cls.state != ca_dedicated) {
#endif
		VID_Shutdown();
	}
}