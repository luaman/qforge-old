/*
	cl_main.c

	client main loop

	Copyright (C) 1996-1997  Id Software, Inc.
	Copyright (C) 1999,2000  Nelson Rush.
	Copyright (C) 1999,2000  contributors of the QuakeForge project
	Please see the file "AUTHORS" for a list of contributors

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
		Boston, MA  02111-1307, USA

	$Id$
*/

/* Will have problems w/ isspace() when compiling w/ gcc on SGI without this */
#ifdef sgi
#define _LINT
#define UNDEFINE_LINT
#endif

#include <ctype.h>

#ifdef UNDEFINE_LINT
#undef _LINT
#endif

#ifdef _WIN32
# include <winsock2.h>
# include <ws2tcpip.h>
# ifdef HAVE_IPV6
#  include <tpipv6.h>
# endif
# define  _WINSOCKAPI_
# define HAVE_SOCKLEN_T
#else
#include <sys/types.h>
#include <netinet/in.h>
#endif

#ifdef __sun
/* Sun's model_t in sys/model.h conflicts w/ Quake's model_t */
#define model_t sunmodel_t
#endif

#include <quakedef.h>
#include <winquake.h>
#ifdef QUAKEWORLD
#include <pmove_simple.h>
#endif
#include <qtypes.h>
#include <qstructs.h>
#include <client.h>
#include <menu.h>
#include <sound.h>
#include <common.h>
#include <net.h>
#include <console.h>
#include <screen.h>
#include <mathlib.h>
#include <keys.h>
#include <sound.h>
#include <cmd.h>
#include <protocol.h>
#include <cvar.h>
#include <screen.h>
#include <zone.h>
#include <client.h>
#ifdef UQUAKE
#include <server.h>
#endif
#include <view.h>
#include <sbar.h>
#include <cdaudio.h>
#include <input.h>
#ifdef QUAKEWORLD
#include <cl_slist.h>
#endif
#ifdef __sun
#undef model_t
#endif

// we need to declare some mouse variables here, because the menu system
// references them even when on a unix system.

qboolean	noclip_anglehack;		// remnant from old quake


cvar_t	*rcon_password;
cvar_t	*rcon_address;
cvar_t	*cl_timeout;

cvar_t	*cl_autoexec;

// these two are not intended to be set directly
cvar_t	*cl_name;
cvar_t	*cl_color;

cvar_t	*cl_shownet;
cvar_t	*cl_nolerp;

cvar_t	*cl_sbar;
cvar_t	*cl_hudswap;
cvar_t	*cl_maxfps;

cvar_t	*lookspring;
cvar_t	*lookstrafe;
cvar_t	*sensitivity;

cvar_t	*m_pitch;
cvar_t	*m_yaw;
cvar_t	*m_forward;
cvar_t	*m_side;

cvar_t	*entlatency;
cvar_t	*cl_predict_players;
cvar_t	*cl_predict_players2;
cvar_t	*cl_solid_players;
cvar_t	*cl_verstring;

cvar_t	*cl_talksound;
cvar_t	*cl_bonusflash;
cvar_t	*cl_muzzleflash;
cvar_t	*cl_rocketlight;

extern cvar_t	*sys_nostdout;

cvar_t	*localid;

#ifdef QUAKEWORLD
static qboolean allowremotecmd = true;
// Need this defined here for server list loading
extern cvar_t *fs_basepath;
#endif

//
// info mirrors
//
cvar_t	*password;
cvar_t	*spectator;
cvar_t	*name;
cvar_t	*team;
cvar_t	*skin;
cvar_t	*topcolor;
cvar_t	*bottomcolor;
cvar_t	*rate;
cvar_t	*noaim;
cvar_t	*msg;

extern cvar_t	*cl_hightrack;

client_static_t	cls;
client_state_t	cl;
entity_state_t	cl_baselines[MAX_EDICTS];
// FIXME: put these on hunk?
efrag_t			cl_efrags[MAX_EFRAGS];
entity_t		cl_static_entities[MAX_STATIC_ENTITIES];
lightstyle_t	cl_lightstyle[MAX_LIGHTSTYLES];
dlight_t		cl_dlights[MAX_DLIGHTS];

// refresh list
// this is double buffered so the last frame
// can be scanned for oldorigins of trailing objects
int				cl_numvisedicts, cl_oldnumvisedicts;
entity_t		*cl_visedicts, *cl_oldvisedicts;
entity_t		cl_visedicts_list[2][MAX_VISEDICTS];

double			connect_time = -1;		// for connection retransmits

quakeparms_t host_parms;

qboolean	host_initialized;		// true if into command execution
qboolean	nomaster;

double		host_frametime;
int			host_framecount;

int			host_hunklevel;

byte		*host_basepal;
byte		*host_colormap;

netadr_t	master_adr;				// address of the master server

cvar_t	*host_speeds;
cvar_t	*show_fps;

void Master_Connect_f (void);

char	*server_version = NULL;	// version of server we connected to

char emodel_name[] = "emodel";
char pmodel_name[] = "pmodel";
char prespawn_name[] = "prespawn %i 0 %i";
char modellist_name[] = "modellist %i %i";
char soundlist_name[] = "soundlist %i %i";


#ifdef QUAKEWORLD
void
CL_BeginServerConnect ( void )
{
	connect_time = 0;
	CL_CheckForResend();
}
#endif

/*
=================
CL_Changing_f

Just sent as a hint to the client that they should
drop to full console
=================
*/
void
CL_Changing_f ( void )
{
#ifdef QUAKEWORLD
	if (cls.download)	// don't change when downloading
		return;
#endif

	S_StopAllSounds (true);
	cl.intermission = 0;
	cls.state = ca_connected;	// not active anymore, but not disconnected
	Con_Printf ("\nChanging map...\n");
}

#ifdef QUAKEWORLD
/*
=================
CL_CheckForResend

Resend a connect message if the last one has timed out

=================
*/
void
CL_CheckForResend ( void )
{
	netadr_t	adr;
	char	data[2048];
	double t1, t2;

	if (connect_time == -1)
		return;
	if (cls.state != ca_disconnected)
		return;
	if (connect_time && realtime - connect_time < 5.0)
		return;

	t1 = Sys_DoubleTime ();
	if (!NET_StringToAdr (cls.servername, &adr))
	{
		Con_Printf ("Bad server address\n");
		connect_time = -1;
		return;
	}
	if (!NET_IsClientLegal(&adr))
	{
		Con_Printf ("Illegal server address\n");
		connect_time = -1;
		return;
	}

	if (adr.port == 0)
		adr.port = BigShort (27500);
	t2 = Sys_DoubleTime ();

	connect_time = realtime+t2-t1;	// for retransmit requests

	Con_Printf ("Connecting to %s...\n", cls.servername);
	snprintf(data, sizeof(data), "%c%c%c%cgetchallenge\n", 255, 255, 255, 255);
	NET_SendPacket (strlen(data), data, adr);
}
#endif

/*
=====================
CL_ClearState

=====================
*/
void
CL_ClearState ( void )
{
	int			i;

	S_StopAllSounds (true);

#ifdef QUAKEWORLD
	Host_ClearMemory ();
#elif UQUAKE
	if (!sv.active)
		Host_ClearMemory ();
#endif
	CL_ClearTEnts ();

// wipe the entire cl structure
	memset (&cl, 0, sizeof(cl));

	SZ_Clear (&cls.netchan.message);

// clear other arrays
	memset (cl_efrags, 0, sizeof(cl_efrags));
#ifdef UQUAKE
	memset (cl_entities, 0, sizeof(cl_entities));
	memset (cl_temp_entities, 0, sizeof(cl_temp_entities));
#endif
	memset (cl_dlights, 0, sizeof(cl_dlights));
	memset (cl_lightstyle, 0, sizeof(cl_lightstyle));
	CL_ClearTEnts ();

//
// allocate the efrags and chain together into a free list
//
	cl.free_efrags = cl_efrags;
	for (i=0 ; i<MAX_EFRAGS-1 ; i++)
		cl.free_efrags[i].entnext = &cl.free_efrags[i+1];
	cl.free_efrags[i].entnext = NULL;
}

#ifdef QUAKEWORLD
void
CL_Color_f ( void )
{
	// just for quake compatability...
	int		top, bottom;
	char	num[16];

	if (Cmd_Argc() == 1)
	{
		Con_Printf ("\"color\" is \"%s %s\"\n",
			Info_ValueForKey (cls.userinfo, "topcolor"),
			Info_ValueForKey (cls.userinfo, "bottomcolor") );
		Con_Printf ("color <0-13> [0-13]\n");
		return;
	}

	if (Cmd_Argc() == 2)
		top = bottom = atoi(Cmd_Argv(1));
	else
	{
		top = atoi(Cmd_Argv(1));
		bottom = atoi(Cmd_Argv(2));
	}

	top &= 15;
	if (top > 13)
		top = 13;
	bottom &= 15;
	if (bottom > 13)
		bottom = 13;

	snprintf(num, sizeof(num), "%i", top);
	Cvar_Set (topcolor, num);
	snprintf(num, sizeof(num), "%i", bottom);
	Cvar_Set (bottomcolor, num);
}

/*
================
CL_Connect_f

================
*/
void
CL_Connect_f ( void )
{
	char	*server;
#ifdef QUAKEWORLD
	int snum;
#endif

	if (Cmd_Argc() != 2) {
		Con_Printf ("usage: connect <server>\n");
		return;
	}

	server = Cmd_Argv (1);
#ifdef QUAKEWORLD
	if (server[0] == '#') {
		snum = Q_atoi(server + 1);
		if (snum >= MAX_SERVER_LIST || !(slist[snum].server)) {
			Con_Printf("Server not found.\n");
			return;
		}
		server = slist[snum].server;
	}
#endif
	CL_Disconnect ();

	strncpy (cls.servername, server, sizeof(cls.servername)-1);
	CL_BeginServerConnect();
}

/*
=================
CL_ConnectionlessPacket

Responses to broadcasts, etc
=================
*/
void
CL_ConnectionlessPacket ( void )
{
	char	*s;
	int		c;

	MSG_BeginReading ();
	MSG_ReadLong ();		// skip the -1

	c = MSG_ReadByte ();
	if (!cls.demoplayback)
		Con_Printf ("%s: ", NET_AdrToString (net_from));
//	Con_DPrintf ("%s", net_message.data + 5);
	if (c == S2C_CONNECTION)
	{
		Con_Printf ("connection\n");
		if (cls.state >= ca_connected)
		{
			if (!cls.demoplayback)
				Con_Printf ("Dup connect received.  Ignored.\n");
			return;
		}
		Netchan_Setup (&cls.netchan, net_from, cls.qport);
		MSG_WriteChar (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message, "new");
		cls.state = ca_connected;
		Con_Printf ("Connected.\n");
		allowremotecmd = false;		// localid required now for remote cmds
		return;
	}
	// remote command from gui front end
	if (c == A2C_CLIENT_COMMAND)
	{
		char	cmdtext[2048];

		Con_Printf ("client command\n");

		if (!NET_AdrIsLoopback(net_from) && !NET_CompareBaseAdr(net_from,net_local_adr)) {
			Con_Printf ("Command packet from remote host.  Ignored.\n");
			return;
		}

#ifdef _WIN32
		ShowWindow (mainwindow, SW_RESTORE);
		SetForegroundWindow (mainwindow);
#endif
		s = MSG_ReadString ();

		strncpy(cmdtext, s, sizeof(cmdtext) - 1);
		cmdtext[sizeof(cmdtext) - 1] = 0;

		s = MSG_ReadString ();

		while (*s && isspace((int)*s))
			s++;
		while (*s && isspace((int)s[strlen(s) - 1]))
			s[strlen(s) - 1] = 0;

		if (!allowremotecmd && (!*localid->string || strcmp(localid->string, s))) {
			if (!*localid->string) {
				Con_Printf("===========================\n");
				Con_Printf("Command packet received from local host, but no "
					"localid has been set.  You may need to upgrade your server "
					"browser.\n");
				Con_Printf("===========================\n");
				return;
			}
			Con_Printf("===========================\n");
			Con_Printf("Invalid localid on command packet received from local host. "
				"\n|%s| != |%s|\n"
				"You may need to reload your server browser and QuakeForge.\n",
				s, localid->string);
			Con_Printf("===========================\n");
			Cvar_Set(localid, "");
			return;
		}

		Cbuf_AddText (cmdtext);
		allowremotecmd = false;
		return;
	}
	// print command from somewhere
	if (c == A2C_PRINT)
	{
		Con_Printf ("print\n");

		s = MSG_ReadString ();
		Con_Print (s);
		return;
	}

	// ping from somewhere
	if (c == A2A_PING)
	{
		char	data[6];

		Con_Printf ("ping\n");

		data[0] = 0xff;
		data[1] = 0xff;
		data[2] = 0xff;
		data[3] = 0xff;
		data[4] = A2A_ACK;
		data[5] = 0;

		NET_SendPacket (6, data, net_from);
		return;
	}

	if (c == S2C_CHALLENGE) {
		Con_Printf ("challenge\n");

		s = MSG_ReadString ();
		cls.challenge = atoi(s);
		CL_SendConnectPacket ();
		return;
	}

	if (c == svc_disconnect) {
		if (cls.demoplayback)
			Host_EndGame ("End of demo");
		else
			Con_Printf ("svc_disconnect\n");
//		Host_EndGame ("Server disconnected");
		return;
	}

	Con_Printf ("unknown:  %c\n", c);
}
#endif


/*
=====================
CL_Disconnect

Sends a disconnect message to the server
This is also called on Host_Error, so it shouldn't cause any errors
=====================
*/
void
CL_Disconnect ( void )
{
#ifdef QUAKEWORLD
	byte	final[10];
#endif

	connect_time = -1;

#if defined(_WIN32) && defined (QUAKEWORLD)
	SetWindowText (mainwindow, "QuakeWorld: disconnected");
#endif

// stop sounds (especially looping!)
	S_StopAllSounds (true);

// bring the console down and fade the colors back to normal
//	SCR_BringDownConsole ();

// if running a local server, shut it down
	if (cls.demoplayback)
		CL_StopPlayback ();
	else if (cls.state != ca_disconnected)
//	else if (cls.state >= ca_connected)
	{
		if (cls.demorecording)
			CL_Stop_f ();

#ifdef QUAKEWORLD
		final[0] = clc_stringcmd;
		strcpy (final+1, "drop");
		Netchan_Transmit (&cls.netchan, 6, final);
		Netchan_Transmit (&cls.netchan, 6, final);
		Netchan_Transmit (&cls.netchan, 6, final);
#else
		Con_DPrintf ("Sending clc_disconnect\n");
		SZ_Clear (&cls.netchan.message);
		MSG_WriteByte (&cls.netchan.message, clc_disconnect);
		NET_SendUnreliableMessage (cls.netcon, &cls.netchan.message);
		SZ_Clear (&cls.netchan.message);
		NET_Close (cls.netcon);
#endif

		cls.state = ca_disconnected;

		cls.demoplayback = cls.demorecording = cls.timedemo = false;
#ifdef UQUAKE
		if (sv.active)
			SV_Shutdown(false);
#endif
	}
#ifdef QUAKEWORLD
	Cam_Reset();

	if (cls.download) {
		Qclose(cls.download);
		cls.download = NULL;
	}

	CL_StopUpload();
#endif
#ifdef UQUAKE
	cls.demoplayback = cls.timedemo = false;
	cls.signon = 0;
	cl.paused = false;	// Tonik
#endif
}

void
CL_Disconnect_f ( void )
{
	CL_Disconnect ();
#ifdef UQUAKE
	if (sv.active)
		SV_Shutdown (false);
#endif
}

#ifdef QUAKEWORLD
/*
=====================
CL_Download_f
=====================
*/
void
CL_Download_f ( void )
{
	char *p, *q;

	if (cls.state == ca_disconnected)
	{
		Con_Printf ("Must be connected.\n");
		return;
	}

	if (Cmd_Argc() != 2)
	{
		Con_Printf ("Usage: download <datafile>\n");
		return;
	}

	snprintf(cls.downloadname, sizeof(cls.downloadname), "%s/%s", com_gamedir, Cmd_Argv(1));

	p = cls.downloadname;
	while ((q = strchr(p, '/')) != NULL)
	{
		*q = 0;
		Sys_mkdir(cls.downloadname);
		*q = '/';
		p = q + 1;
	}

	strcpy(cls.downloadtempname, cls.downloadname);
	cls.download = Qopen (cls.downloadname, "wb");
	cls.downloadtype = dl_single;

	MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
	SZ_Print (&cls.netchan.message, va("download %s\n",Cmd_Argv(1)));
}
#endif

/*
=====================
CL_EstablishConnection

Host should be either "local" or a net address to be passed on
=====================
*/
void
CL_EstablishConnection ( char *host )
{
	if (cls.state == ca_dedicated)
		return;

	if (cls.demoplayback)
		return;

	CL_Disconnect ();

#ifdef UQUAKE
	cls.netcon = NET_Connect (host);
	if (!cls.netcon)
		Host_Error ("CL_Connect: connect failed\n");
#endif
	Con_DPrintf ("CL_EstablishConnection: connected to %s\n", host);

	cls.demonum = -1;			// not in the demo loop now
	cls.state = ca_connected;
#ifdef UQUAKE
	cls.signon = 0;				// need all the signon messages before playing
#endif
}

/*
==================
CL_FullInfo_f

Allow clients to change userinfo
==================
*/
void
CL_FullInfo_f ( void )
{
	char	key[512];
	char	value[512];
	char	*o;
	char	*s;

	if (Cmd_Argc() != 2)
	{
		Con_Printf ("fullinfo <complete info string>\n");
		return;
	}

	s = Cmd_Argv(1);
	if (*s == '\\')
		s++;
	while (*s)
	{
		o = key;
		while (*s && *s != '\\')
			*o++ = *s++;
		*o = 0;

		if (!*s)
		{
			Con_Printf ("MISSING VALUE\n");
			return;
		}

		o = value;
		s++;
		while (*s && *s != '\\')
			*o++ = *s++;
		*o = 0;

		if (*s)
			s++;

		if (!stricmp(key, pmodel_name) || !stricmp(key, emodel_name))
			continue;

#ifdef QUAKEWORLD
		Info_SetValueForKey (cls.userinfo, key, value, MAX_INFO_STRING);
#endif
	}
}

#ifdef QUAKEWORLD
/*
==================
CL_FullServerinfo_f

Sent by server when serverinfo changes
==================
*/
void
CL_FullServerinfo_f ( void )
{
	char *p;

	if (Cmd_Argc() != 2)
	{
		Con_Printf ("usage: fullserverinfo <complete info string>\n");
		return;
	}

	Con_DPrintf("Cmd_Argv(1): '%s'\n", Cmd_Argv(1));
	strcpy (cl.serverinfo, Cmd_Argv(1));
	Con_DPrintf("cl.serverinfo: '%s'\n", cl.serverinfo);

	if ((p = Info_ValueForKey(cl.serverinfo, "*qf_version")) && *p) {
		if (!server_version)
			Con_Printf("QuakeForge Version %s Server\n", p);
		server_version = strdup(p);
	} else if ((p = Info_ValueForKey(cl.serverinfo, "*version")) && *p) {
		if (!server_version)
			Con_Printf("Version %s Server\n", p);
		server_version = strdup(p);
	}

	if ((p = Info_ValueForKey(cl.serverinfo, "*qsg_standard")) && *p)
	{
		if ((cl.stdver = Q_atoi (p)))
			Con_Printf("QSG standards version %i\n", cl.stdver);
		else
			Con_Printf("Invalid standards version: %s", p);
	}

// check game type (also done in CL_ServerInfo_f)
	if ((p = Info_ValueForKey(cl.serverinfo, "deathmatch")) && *p)
		cl.gametype = Q_atoi (p) ? GAME_DEATHMATCH : GAME_COOP;
	else
		cl.gametype = GAME_DEATHMATCH;
}
#endif

#ifdef UQUAKE
/*
===============
CL_LerpPoint

Determines the fraction between the last two messages that the objects
should be put at.
===============
*/
float
CL_LerpPoint ( void )
{
	float	f, frac;

	f = cl.mtime[0] - cl.mtime[1];

	if (!f || cl_nolerp->value || cls.timedemo || sv.active)
	{
		cl.time = cl.mtime[0];
		return 1;
	}
	if (f > 0.1)
	{	// dropped packet, or start of demo
		cl.mtime[1] = cl.mtime[0] - 0.1;
		f = 0.1;
	}
	frac = (cl.time - cl.mtime[1]) / f;
//	Con_Printf ("frac: %f\n",frac);
	if (frac < 0)
	{
		if (frac < -0.01)
		{
SetPal(1);
			cl.time = cl.mtime[1];
//			Con_Printf ("low frac\n");
		}
		frac = 0;
	}
	else if (frac > 1)
	{
		if (frac > 1.01)
		{
SetPal(2);
			cl.time = cl.mtime[0];
//			Con_Printf ("high frac\n");
		}
		frac = 1;
	}
	else
		SetPal(0);

	return frac;
}
#endif

/*
=====================
CL_NextDemo

Called to play the next demo in the demo loop
=====================
*/
void
CL_NextDemo ( void )
{
	char	str[1024];

	if (cls.demonum == -1)
		return;		// don't play demos

#ifdef UQUAKE
	SCR_BeginLoadingPlaque ();
#endif

	if (!cls.demos[cls.demonum][0] || cls.demonum == MAX_DEMOS)
	{
		cls.demonum = 0;
		if (!cls.demos[cls.demonum][0])
		{
			Con_Printf ("No demos listed with startdemos\n");
			cls.demonum = -1;
			return;
		}
	}

	snprintf(str, sizeof(str),"playdemo %s\n", cls.demos[cls.demonum]);
	Cbuf_InsertText (str);
	cls.demonum++;
}

#ifdef QUAKEWORLD
/*
====================
CL_Packet_f

packet <destination> <contents>

Contents allows \n escape character
====================
*/
void
CL_Packet_f ( void )
{
	char	send[2048];
	int		i, l;
	char	*in, *out;
	netadr_t	adr;

	if (Cmd_Argc() != 3)
	{
		Con_Printf ("packet <destination> <contents>\n");
		return;
	}

	if (!NET_StringToAdr (Cmd_Argv(1), &adr))
	{
		Con_Printf ("Bad address\n");
		return;
	}

	in = Cmd_Argv(2);
	out = send+4;
	send[0] = send[1] = send[2] = send[3] = 0xff;

	l = strlen (in);
	for (i=0 ; i<l ; i++)
	{
		if (in[i] == '\\' && in[i+1] == 'n')
		{
			*out++ = '\n';
			i++;
		}
		else
			*out++ = in[i];
	}
	*out = 0;

	NET_SendPacket (out-send, send, adr);
}
#endif

#ifdef UQUAKE
/*
==============
CL_PrintEntities_f
==============
*/
void
CL_PrintEntities_f ( void )
{
	entity_t	*ent;
	int			i;

	for (i=0,ent=cl_entities ; i<cl.num_entities ; i++,ent++)
	{
		Con_Printf ("%3i:",i);
		if (!ent->model)
		{
			Con_Printf ("EMPTY\n");
			continue;
		}
		Con_Printf ("%s:%2i  (%5.1f,%5.1f,%5.1f) [%5.1f %5.1f %5.1f]\n"
		,ent->model->name,ent->frame, ent->origin[0], ent->origin[1], ent->origin[2], ent->angles[0], ent->angles[1], ent->angles[2]);
	}
}
#endif

/*
==================
CL_Quit_f
==================
*/
void
CL_Quit_f ( void )
{
#ifdef QUAKEWORLD
	if (1 /* key_dest != key_console */ /* && cls.state != ca_dedicated */)
	{
		M_Menu_Quit_f ();
		return;
	}
#endif
	CL_Disconnect ();
	Sys_Quit ();
}

#ifdef QUAKEWORLD
/*
=====================
CL_Rcon_f

  Send the rest of the command line over as
  an unconnected command.
=====================
*/
void
CL_Rcon_f ( void )
{
	char	message[1024];
	int		i;
	netadr_t	to;

	if (!rcon_password->string)
	{
		Con_Printf ("You must set 'rcon_password' before\n"
					"issuing an rcon command.\n");
		return;
	}

	message[0] = 255;
	message[1] = 255;
	message[2] = 255;
	message[3] = 255;
	message[4] = 0;

	strcat (message, "rcon ");

	strcat (message, rcon_password->string);
	strcat (message, " ");

	for (i=1 ; i<Cmd_Argc() ; i++)
	{
		strcat (message, Cmd_Argv(i));
		strcat (message, " ");
	}

	if (cls.state >= ca_connected)
		to = cls.netchan.remote_address;
	else
	{
		if (!strlen(rcon_address->string))
		{
			Con_Printf ("You must either be connected,\n"
						"or set the 'rcon_address' cvar\n"
						"to issue rcon commands\n");

			return;
		}
		NET_StringToAdr (rcon_address->string, &to);
	}

	NET_SendPacket (strlen(message)+1, message
		, to);
}
#endif

#ifdef UQUAKE
/*
===============
CL_ReadFromServer

Read all incoming data from the server
===============
*/
int
CL_ReadFromServer ( void )
{
	int		ret;

	cl.oldtime = cl.time;
	cl.time += host_frametime;

	do
	{
		ret = CL_GetMessage ();
		if (ret == -1)
			Host_Error ("CL_ReadFromServer: lost server connection");
		if (!ret)
			break;

		cl.last_received_message = realtime;
		CL_ParseServerMessage ();
	} while (ret && cls.state >= ca_connected);

	if (cl_shownet->value)
		Con_Printf ("\n");

	CL_RelinkEntities ();
	CL_UpdateTEnts ();

//
// bring the links up to date
//
	return 0;
}
#endif

#ifdef QUAKEWORLD
/*
=================
CL_ReadPackets
=================
*/
void
CL_ReadPackets ( void )
{
//	while (NET_GetPacket ())
	while (CL_GetMessage())
	{
		//
		// remote command packet
		//
		if (*(int *)net_message.data == -1)
		{
			CL_ConnectionlessPacket ();
			continue;
		}

		if (net_message.cursize < 8)
		{
			Con_Printf ("%s: Runt packet\n",NET_AdrToString(net_from));
			continue;
		}

		//
		// packet from server
		//
		if (!cls.demoplayback &&
			!NET_CompareAdr (net_from, cls.netchan.remote_address))
		{
			Con_DPrintf ("%s:sequenced packet without connection\n"
				,NET_AdrToString(net_from));
			continue;
		}
		if (!Netchan_Process(&cls.netchan))
			continue;		// wasn't accepted for some reason
		CL_ParseServerMessage ();

//		if (cls.demoplayback && cls.state >= ca_active && !CL_DemoBehind())
//			return;
	}

	//
	// check timeout
	//
	if (cls.state >= ca_connected
	 && realtime - cls.netchan.last_received > cl_timeout->value)
	{
		Con_Printf ("\nServer connection timed out.\n");
		CL_Disconnect ();
		return;
	}

}
#endif

/*
=================
CL_Reconnect_f

The server is changing levels
=================
*/
void
CL_Reconnect_f ( void )
{
#ifdef QUAKEWORLD
	if (cls.download)	// don't change when downloading
		return;
#endif

	S_StopAllSounds (true);

	if (cls.state == ca_connected) {
		Con_Printf ("reconnecting...\n");
		MSG_WriteChar (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message, "new");
		return;
	}

#ifdef QUAKEWORLD
	if (!*cls.servername) {
		Con_Printf("No server to reconnect to...\n");
		return;
	}
#endif

	CL_Disconnect();
#ifdef QUAKEWORLD
	CL_BeginServerConnect();
#endif
}

#ifdef UQUAKE
/*
===============
CL_RelinkEntities
===============
*/
void
CL_RelinkEntities ( void )
{
	entity_t	*ent;
	int			i, j;
	float		frac, f, d;
	vec3_t		delta;
	float		bobjrotate;
	vec3_t		oldorg;
	dlight_t	*dl;

// determine partial update time
	frac = CL_LerpPoint ();

	cl_numvisedicts = 0;

//
// interpolate player info
//
	for (i=0 ; i<3 ; i++)
		cl.velocity[i] = cl.mvelocity[1][i] +
			frac * (cl.mvelocity[0][i] - cl.mvelocity[1][i]);

	if (cls.demoplayback)
	{
	// interpolate the angles
		for (j=0 ; j<3 ; j++)
		{
			d = cl.mviewangles[0][j] - cl.mviewangles[1][j];
			if (d > 180)
				d -= 360;
			else if (d < -180)
				d += 360;
			cl.viewangles[j] = cl.mviewangles[1][j] + frac*d;
		}
	}

	bobjrotate = anglemod(100*cl.time);

// start on the entity after the world
	for (i=1,ent=cl_entities+1 ; i<cl.num_entities ; i++,ent++)
	{
		if (!ent->model)
		{	// empty slot
			if (ent->forcelink)
				R_RemoveEfrags (ent);	// just became empty
			continue;
		}

// if the object wasn't included in the last packet, remove it
		if (ent->msgtime != cl.mtime[0])
		{
			ent->model = NULL;
#ifdef EXPERIMENTAL
			ent->translate_start_time = 0;
			ent->rotate_start_time = 0;
#endif
			continue;
		}

		VectorCopy (ent->origin, oldorg);

		if (ent->forcelink)
		{	// the entity was not updated in the last message
			// so move to the final spot
			VectorCopy (ent->msg_origins[0], ent->origin);
			VectorCopy (ent->msg_angles[0], ent->angles);
		}
		else
		{	// if the delta is large, assume a teleport and don't lerp
			f = frac;
			for (j=0 ; j<3 ; j++)
			{
				delta[j] = ent->msg_origins[0][j] - ent->msg_origins[1][j];
				if (delta[j] > 100 || delta[j] < -100)
					f = 1;		// assume a teleportation, not a motion
			}

		// interpolate the origin and angles
			for (j=0 ; j<3 ; j++)
			{
				ent->origin[j] = ent->msg_origins[1][j] + f*delta[j];

				d = ent->msg_angles[0][j] - ent->msg_angles[1][j];
				if (d > 180)
					d -= 360;
				else if (d < -180)
					d += 360;
				ent->angles[j] = ent->msg_angles[1][j] + f*d;
			}

		}

// rotate binary objects locally
		if (ent->model->flags & EF_ROTATE)
			ent->angles[1] = bobjrotate;

		if (ent->effects & EF_BRIGHTFIELD)
			R_EntityParticles (ent);
#ifdef QUAKE2
		if (ent->effects & EF_DARKFIELD)
			R_DarkFieldParticles (ent);
#endif
		if (ent->effects & EF_MUZZLEFLASH)
		{
			vec3_t		fv, rv, uv;

			if (!cl_muzzleflash->value || (cl_muzzleflash->value == 2 && ent == &cl_entities[cl.playernum+1]))
				return;

			dl = CL_AllocDlight (i);
			VectorCopy (ent->origin, dl->origin);
			dl->origin[2] += 16;
			AngleVectors (ent->angles, fv, rv, uv);

			VectorMA (dl->origin, 18, fv, dl->origin);
			dl->radius = 200 + (rand()&31);
			dl->minlight = 32;
			dl->die = cl.time + 0.1;
		}
		if (ent->effects & EF_BRIGHTLIGHT)
		{
			dl = CL_AllocDlight (i);
			VectorCopy (ent->origin, dl->origin);
			dl->origin[2] += 16;
			dl->radius = 400 + (rand()&31);
			dl->die = cl.time + 0.001;
		}
		if (ent->effects & EF_DIMLIGHT)
		{
			dl = CL_AllocDlight (i);
			VectorCopy (ent->origin, dl->origin);
			dl->radius = 200 + (rand()&31);
			dl->die = cl.time + 0.001;
		}
#ifdef QUAKE2
		if (ent->effects & EF_DARKLIGHT)
		{
			dl = CL_AllocDlight (i);
			VectorCopy (ent->origin, dl->origin);
			dl->radius = 200.0 + (rand()&31);
			dl->die = cl.time + 0.001;
			dl->dark = true;
		}
		if (ent->effects & EF_LIGHT)
		{
			dl = CL_AllocDlight (i);
			VectorCopy (ent->origin, dl->origin);
			dl->radius = 200;
			dl->die = cl.time + 0.001;
		}
#endif

		if (ent->model->flags & EF_GIB)
			R_RocketTrail (oldorg, ent->origin, 2, ent);
		else if (ent->model->flags & EF_ZOMGIB)
			R_RocketTrail (oldorg, ent->origin, 4, ent);
		else if (ent->model->flags & EF_TRACER)
			R_RocketTrail (oldorg, ent->origin, 3, ent);
		else if (ent->model->flags & EF_TRACER2)
			R_RocketTrail (oldorg, ent->origin, 5, ent);
		else if (ent->model->flags & EF_ROCKET)
		{
			R_RocketTrail (oldorg, ent->origin, 0, ent);
			if (cl_rocketlight->value)
			{
				dl = CL_AllocDlight (i);
				VectorCopy (ent->origin, dl->origin);
				dl->radius = 200;
				dl->die = cl.time + 0.01;
			}
		}
		else if (ent->model->flags & EF_GRENADE)
			R_RocketTrail (oldorg, ent->origin, 1, ent);
		else if (ent->model->flags & EF_TRACER3)
			R_RocketTrail (oldorg, ent->origin, 6, ent);

		ent->forcelink = false;

		if (i == cl.playernum + 1 && !cl_chasecam->value)
			continue;

#ifdef QUAKE2
		if ( ent->effects & EF_NODRAW )
			continue;
#endif
		if (cl_numvisedicts < MAX_VISEDICTS)
		{
			cl_visedicts[cl_numvisedicts] = *ent;
			cl_numvisedicts++;
		}
	}

}
#endif

#ifdef QUAKEWORLD
/*
=======================
CL_SendConnectPacket

called by CL_Connect_f and CL_CheckResend
======================
*/
void
CL_SendConnectPacket ( void )
{
	netadr_t	adr;
	char	data[2048];
	double t1, t2;
// JACK: Fixed bug where DNS lookups would cause two connects real fast
//       Now, adds lookup time to the connect time.
//		 Should I add it to realtime instead?!?!

	if (cls.state != ca_disconnected)
		return;

	t1 = Sys_DoubleTime ();
	if (!NET_StringToAdr (cls.servername, &adr))
	{
		Con_Printf ("Bad server address\n");
		connect_time = -1;
		return;
	}
	if (!NET_IsClientLegal(&adr))
	{
		Con_Printf ("Illegal server address\n");
		connect_time = -1;
		return;
	}

	if (adr.port == 0)
		adr.port = BigShort (27500);
	t2 = Sys_DoubleTime ();

	connect_time = realtime+t2-t1;	// for retransmit requests
	cls.qport = Cvar_VariableValue("qport");

	// Arrgh, this was not in the old binary only release, and eats up
	// far too much of the 196 chars in the userinfo space, leaving nothing
	// for player use, thus, its commented out for the moment..
	//
	//Info_SetValueForStarKey (cls.userinfo, "*ip", NET_AdrToString(adr), MAX_INFO_STRING);

//	Con_Printf ("Connecting to %s...\n", cls.servername);
	snprintf(data, sizeof(data), "%c%c%c%cconnect %i %i %i \"%s\"\n",
		255, 255, 255, 255,	PROTOCOL_VERSION, cls.qport, cls.challenge, cls.userinfo);
	NET_SendPacket (strlen(data), data, adr);
}
#endif

#ifdef QUAKEWORLD
/*
==================
CL_SetInfo_f

Allow clients to change userinfo
==================
*/
void
CL_SetInfo_f ( void )
{
	if (Cmd_Argc() == 1)
	{
		Info_Print (cls.userinfo);
		return;
	}
	if (Cmd_Argc() != 3)
	{
		Con_Printf ("usage: setinfo [ <key> <value> ]\n");
		return;
	}
	if (!stricmp(Cmd_Argv(1), pmodel_name) || !strcmp(Cmd_Argv(1), emodel_name))
		return;

	Info_SetValueForKey (cls.userinfo, Cmd_Argv(1), Cmd_Argv(2), MAX_INFO_STRING);
	if (cls.state >= ca_connected)
		Cmd_ForwardToServer ();
}
#endif

#ifdef UQUAKE
/*
=====================
CL_SignonReply

An svc_signonnum has been received, perform a client side setup
=====================
*/
void
CL_SignonReply ( void )
{
	char 	str[8192];

	Con_DPrintf ("CL_SignonReply: %i\n", cls.signon);

	switch (cls.signon)
	{
	case 1:
		MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message, "prespawn");
		cls.state = ca_onserver;
		break;

	case 2:
		MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message, va("name \"%s\"\n", cl_name->string));

		MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message, va("color %i %i\n", ((int)cl_color->value)>>4, ((int)cl_color->value)&15));

		MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
		snprintf(str, sizeof(str), "spawn %s", cls.spawnparms);
		MSG_WriteString (&cls.netchan.message, str);
		break;

	case 3:
		MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message, "begin");
		Cache_Report ();		// print remaining memory
		break;

	case 4:
		SCR_EndLoadingPlaque ();		// allow normal screen updates
		cls.state = ca_active;
		break;
	}
}
#endif

#ifdef QUAKEWORLD
/*
====================
CL_User_f

user <name or userid>

Dump userdata / masterdata for a user
====================
*/
void
CL_User_f ( void )
{
	int		uid;
	int		i;

	if (Cmd_Argc() != 2)
	{
		Con_Printf ("Usage: user <username / userid>\n");
		return;
	}

	uid = atoi(Cmd_Argv(1));

	for (i=0 ; i<MAX_CLIENTS ; i++)
	{
		if (!cl.players[i].name[0])
			continue;
		if (cl.players[i].userid == uid
		|| !strcmp(cl.players[i].name, Cmd_Argv(1)) )
		{
			Info_Print (cl.players[i].userinfo);
			return;
		}
	}
	Con_Printf ("User not in server.\n");
}

/*
====================
CL_Users_f

Dump userids for all current players
====================
*/
void
CL_Users_f ( void )
{
	int		i;
	int		c;

	c = 0;
	Con_Printf ("userid frags name\n");
	Con_Printf ("------ ----- ----\n");
	for (i=0 ; i<MAX_CLIENTS ; i++)
	{
		if (cl.players[i].name[0])
		{
			Con_Printf ("%6i %4i %s\n", cl.players[i].userid, cl.players[i].frags, cl.players[i].name);
			c++;
		}
	}

	Con_Printf ("%i total users\n", c);
}
#endif

/*
=======================
CL_Version_f
======================
*/
void
CL_Version_f ( void )
{
	Con_Printf ("Version %s\n", QF_VERSION);
	Con_Printf ("Exe: "__TIME__" "__DATE__"\n");
}

#ifdef _WIN32
#include <windows.h>
/*
=================
CL_Minimize_f
=================
*/
void
CL_Windows_f ( void )
{
//	if (modestate == MS_WINDOWED)
//		ShowWindow(mainwindow, SW_MINIMIZE);
//	else
		SendMessage(mainwindow, WM_SYSKEYUP, VK_TAB, 1 | (0x0F << 16) | (1<<29));
}
#endif

/*
===============
SetPal

Debugging tool, just flashes the screen
===============
*/
void
SetPal ( int i )
{
#if 0
	static int old;
	byte	pal[768];
	int		c;

	if (i == old)
		return;
	old = i;

	if (i==0)
		VID_SetPalette (host_basepal);
	else if (i==1)
	{
		for (c=0 ; c<768 ; c+=3)
		{
			pal[c] = 0;
			pal[c+1] = 255;
			pal[c+2] = 0;
		}
		VID_SetPalette (pal);
	}
	else
	{
		for (c=0 ; c<768 ; c+=3)
		{
			pal[c] = 0;
			pal[c+1] = 0;
			pal[c+2] = 255;
		}
		VID_SetPalette (pal);
	}
#endif
}

/*
=================
CL_Slist_f
Lists servers in
the server list.
QW only.
=================
*/
#ifdef QUAKEWORLD
void
CL_Slist_f( void )
{
	int i;
	for (i=0;i < MAX_SERVER_LIST;i++) {
		if (slist[i].server)
			Con_Printf("[%i] %s - %s\n",i+1,
			slist[i].description,
			slist[i].server);
	}
}
#endif

/*
=================
CL_Init
=================
*/
void
CL_Init ( void )
{
#ifdef QUAKEWORLD
	QFile *serlist;
#endif
#ifdef UQUAKE
	SZ_Alloc (&cls.netchan.message, 1024);
#endif

	cls.state = ca_disconnected;

#ifdef QUAKEWORLD
	Info_SetValueForKey (cls.userinfo, "name", "unnamed", MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "topcolor", "0", MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "bottomcolor", "0", MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "rate", "2500", MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "msg", "1", MAX_INFO_STRING);
	Info_SetValueForStarKey(cls.userinfo, "*qf_ver", QF_VERSION, MAX_INFO_STRING);
	Info_SetValueForStarKey(cls.userinfo, "*ver", VERSION, MAX_INFO_STRING);
#endif

	CL_InitInput ();
	CL_InitTEnts ();
#ifdef QUAKEWORLD
	CL_InitPrediction ();
	CL_InitCam ();
	Pmove_Init ();
#endif

//
// register our commands
//
	Cmd_AddCommand ("maplist", COM_Maplist_f);
	Cmd_AddCommand ("changing", CL_Changing_f);
#ifdef UQUAKE
	Cmd_AddCommand ("entities", CL_PrintEntities_f);
#endif
	Cmd_AddCommand ("disconnect", CL_Disconnect_f);
	Cmd_AddCommand ("record", CL_Record_f);
#ifdef QUAKEWORLD
	Cmd_AddCommand ("version", CL_Version_f);
	Cmd_AddCommand ("rerecord", CL_ReRecord_f);
#endif
	Cmd_AddCommand ("stop", CL_Stop_f);
	Cmd_AddCommand ("playdemo", CL_PlayDemo_f);
	Cmd_AddCommand ("timedemo", CL_TimeDemo_f);

#ifdef QUAKEWORLD
	Cmd_AddCommand ("skins", Skin_Skins_f);
	Cmd_AddCommand ("allskins", Skin_AllSkins_f);
	Cmd_AddCommand ("quit", CL_Quit_f);

	Cmd_AddCommand ("connect", CL_Connect_f);
	Cmd_AddCommand ("reconnect", CL_Reconnect_f);

	Cmd_AddCommand ("rcon", CL_Rcon_f);
	Cmd_AddCommand ("packet", CL_Packet_f);
	Cmd_AddCommand ("user", CL_User_f);
	Cmd_AddCommand ("users", CL_Users_f);

	Cmd_AddCommand ("setinfo", CL_SetInfo_f);
	Cmd_AddCommand ("fullinfo", CL_FullInfo_f);
	Cmd_AddCommand ("fullserverinfo", CL_FullServerinfo_f);

	Cmd_AddCommand ("color", CL_Color_f);
	Cmd_AddCommand ("download", CL_Download_f);

	Cmd_AddCommand ("nextul", CL_NextUpload);
	Cmd_AddCommand ("stopul", CL_StopUpload);
	Cmd_AddCommand ("kill", NULL);
	Cmd_AddCommand ("pause", NULL);
	Cmd_AddCommand ("say", NULL);
	Cmd_AddCommand ("say_team", NULL);
#endif
//
// forward to server commands
//
	Cmd_AddCommand ("serverinfo", NULL);

//
//  Windows commands
//
#ifdef _WIN32
	Cmd_AddCommand ("windows", CL_Windows_f);
#endif

// New -- Load server list
#ifdef QUAKEWORLD
	Cmd_AddCommand ("slist", CL_Slist_f);

	Server_List_Init();
	printf("CL_Init: Server list initialized.\n");
	if ((serlist = Qopen(va("%s/servers.txt",fs_basepath->string),"r")) != NULL) {
		printf("CL_Init: Found servers.txt...\n");
		Server_List_Load(serlist);
		Qclose(serlist);
	}
#endif
}

void
CL_InitCvars ( void )
{
#ifdef QUAKEWORLD
	extern	cvar_t	*baseskin;
	extern	cvar_t	*noskins;
#endif

//
// register our cvars
//
	host_speeds	= Cvar_Get ("host_speeds","0",0,
		"Toggles the display of host info");
	show_fps	= Cvar_Get ("show_fps","0",0,
		"Toggles the frames-per-second display");
	sys_nostdout	= Cvar_Get ("sys_nostdout","0",0,
		"Toggles the display of console messages on the stdout stream");

	cl_autoexec = Cvar_Get ("cl_autoexec","0",CVAR_ROM,"exec autoexec.cfg on gamedir change");
	cl_warncmd	= Cvar_Get ("cl_warncmd","0",0,"None");
	cl_name		= Cvar_Get ("_cl_name","player",CVAR_ARCHIVE,
		"Sets the player name");
	cl_color	= Cvar_Get ("_cl_color","0",CVAR_ARCHIVE,
		"Sets the player color");
	cl_upspeed	= Cvar_Get ("cl_upspeed","200",0,
		"Sets your vertical speed when in liquids");
	cl_forwardspeed	= Cvar_Get ("cl_forwardspeed","200",CVAR_ARCHIVE,
		"Sets forward movement rate");
	cl_backspeed	= Cvar_Get ("cl_backspeed","200",CVAR_ARCHIVE,
		"Sets backward movement rate");
	cl_sidespeed	= Cvar_Get ("cl_sidespeed","350",0,
		"Sets how quickly you strafe");
	cl_movespeedkey	= Cvar_Get ("cl_movespeedkey","2.0",0,
		"Set multiplier for how fast you move when running");
	cl_yawspeed	= Cvar_Get ("cl_yawspeed","140",0,
		"Sets how quickly you turn left or right");
	cl_pitchspeed	= Cvar_Get ("cl_pitchspeed","150",0,
		"Sets how fast you lookup and lookdown");
	cl_anglespeedkey = Cvar_Get ("cl_anglespeedkey","1.5",0,
		"Sets multiplier for how fast you turn when running");
	cl_shownet	= Cvar_Get ("cl_shownet","0",0,
		"Toggle the display of current net status");
	cl_nolerp	= Cvar_Get ("cl_nolerp","0",0,"None");
	cl_sbar		= Cvar_Get ("cl_sbar","1",CVAR_ARCHIVE,"None");
	cl_hudswap	= Cvar_Get ("cl_hudswap","1",CVAR_ARCHIVE,"None");
	cl_maxfps	= Cvar_Get ("cl_maxfps","0",CVAR_ARCHIVE,
		"Sets the maximum frames-per-second");
	cl_timeout	= Cvar_Get ("cl_timeout","60",0,"None");
	cl_verstring	= Cvar_Get ("cl_verstring",
#ifdef QUAKEWORLD
		"QuakeForge (QW client) " QF_VERSION,
#else
		"QuakeForge (UQuake) " QF_VERSION,
#endif
		CVAR_NONE, "Sets the console version string");
	lookspring	= Cvar_Get ("lookspring","0",CVAR_ARCHIVE,
		"Toggles if the view will recenter after mouselook is "
		"deactivated");
	lookstrafe	= Cvar_Get ("lookstrafe","0",CVAR_ARCHIVE,
		"Toggles if turn left and right will strafe left and right "
		"when mouselook is active");
	sensitivity	= Cvar_Get ("sensitivity","3",CVAR_ARCHIVE,
		"Sets the mouse sensitivity");

	m_pitch		= Cvar_Get ("m_pitch","0.022",CVAR_ARCHIVE,
		"Sets how quickly you lookup and lookdown with the mouse when "			"mouselook is active");
	m_yaw		= Cvar_Get ("m_yaw","0.022",CVAR_ARCHIVE,
		"Sets how quickly you turn left and right with the mouse");
	m_forward	= Cvar_Get ("m_forward","1",CVAR_ARCHIVE,
		"Sets how quickly moving the mouse forwards and backwards "
		"causes the player to move in the respective direction");
	m_side		= Cvar_Get ("m_side","0.8",CVAR_ARCHIVE,
		"Sets how quickly you strafe left and right with the mouse");

	rcon_password	= Cvar_Get ("rcon_password","",0,"None");
	rcon_address	= Cvar_Get ("rcon_address","",0,"None");

	entlatency	= Cvar_Get ("entlatency","20",0,"None");
	cl_predict_players2 = Cvar_Get ("cl_predict_players2","1",0,"None");
	cl_predict_players = Cvar_Get ("cl_predict_players","1",0,
		"Toggles player prediction");
	cl_solid_players = Cvar_Get ("cl_solid_players","1",0,"None");

	cl_talksound	= Cvar_Get ("cl_talksound", "misc/talk.wav", CVAR_NONE,
		"Sets the sound used for talk messages");
	cl_bonusflash	= Cvar_Get ("cl_bonusflash", "1", CVAR_NONE,
		"Toggle light flashes on item pickup");
	cl_muzzleflash	= Cvar_Get ("cl_muzzleflash", "1", CVAR_NONE,
		"Muzzleflashes: 0 - off, 1 - none, 2 - own off");
	cl_rocketlight	= Cvar_Get ("cl_rocketlight", "1", CVAR_NONE,
		"Toggles dynamic rocket lighting effect");

	localid		= Cvar_Get ("localid","",0,"None");

#ifdef QUAKEWORLD
	baseskin	= Cvar_Get ("baseskin","base",0,"None");
	noskins		= Cvar_Get ("noskins","0",0,"None");

	//
	// info mirrors
	//
	name		= Cvar_Get ("name","unnamed",
		CVAR_ARCHIVE|CVAR_USERINFO|CVAR_SERVERINFO,"None");
#endif
	password	= Cvar_Get ("password","",CVAR_USERINFO|CVAR_SERVERINFO,
		"None");
	spectator	= Cvar_Get ("spectator","",
		CVAR_USERINFO|CVAR_SERVERINFO, "None");
	skin		= Cvar_Get ("skin","",
		CVAR_ARCHIVE|CVAR_USERINFO|CVAR_SERVERINFO, "None");
	team		= Cvar_Get ("team","",
		CVAR_ARCHIVE|CVAR_USERINFO|CVAR_SERVERINFO, "None");
	topcolor	= Cvar_Get ("topcolor","0",
		CVAR_ARCHIVE|CVAR_USERINFO|CVAR_SERVERINFO, "None");
	bottomcolor	= Cvar_Get ("bottomcolor","0",
		CVAR_ARCHIVE|CVAR_USERINFO|CVAR_SERVERINFO, "None");
	rate		= Cvar_Get ("rate","2500",
		CVAR_ARCHIVE|CVAR_USERINFO|CVAR_SERVERINFO, "None");
	msg		= Cvar_Get ("msg","1",
		CVAR_ARCHIVE|CVAR_USERINFO|CVAR_SERVERINFO, "None");
	noaim		= Cvar_Get ("noaim","0",
		CVAR_ARCHIVE|CVAR_USERINFO|CVAR_SERVERINFO, "None");
}
