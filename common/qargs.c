/*
qargs.c - command line argument processing routines
Copyright (C) 1996-1997 Id Software, Inc.
Portions Copyright (C) 1999,2000  Nelson Rush.
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
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "config.h"
#ifdef HAVE_MALLOC_H /* QF being compiled with -Werror */
# include <malloc.h> /* causes OpenBSD's build to fail here */
#endif
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <qtypes.h>
#include <common.h>
#include <crc.h>
#include <sys.h>
#include <cmd.h>
#include <console.h>
#include <client.h>
#include <assert.h>
#include "lib_replace.h"

usercmd_t nullcmd; // guarenteed to be zero

static char	**largv;
static char	*argvdummy = " ";

static char	*safeargvs[] =
	{"-stdvid", "-nolan", "-nosound", "-nocdaudio", "-nojoy", "-nomouse", "-dibonly"};

#define NUM_SAFE_ARGVS (sizeof(safeargvs)/sizeof(safeargvs[0]))

int		com_argc;
char	**com_argv;
char	*com_cmdline;
//cvar_t  cmdline = {"cmdline","0", false, true};
cvar_t	*cmdline;

/*
================
COM_CheckParm

Returns the position (1 to argc-1) in the program's argument list
where the given parameter apears, or 0 if not present
================
*/
int COM_CheckParm (char *parm)
{
	int		i;
	
	for (i=1 ; i<com_argc ; i++)
	{
		if (!com_argv[i])
			continue;		// NEXTSTEP sometimes clears appkit vars.
		if (!Q_strcmp (parm,com_argv[i]))
			return i;
	}
		
	return 0;
}

/*
================
COM_InitArgv
================
*/
void COM_InitArgv (int argc, char **argv)
{
	qboolean	safe;
	int			i, len;

	safe = false;

	largv = (char**)malloc ((argc + NUM_SAFE_ARGVS + 1) * sizeof (char**));

	for (com_argc=0, len=0 ; com_argc < argc ; com_argc++)
	{
		largv[com_argc] = argv[com_argc];
		if ((argv[com_argc]) && !Q_strcmp ("-safe", argv[com_argc]))
			safe = true;
		len += strlen (argv[com_argc]) + 1;
	}

	com_cmdline = (char*)malloc (len+1); // need strlen(com_cmdline)+2
	com_cmdline[0] = 0;
	for (i=0; i < argc; i++)
	{
		strncat (com_cmdline, argv[i], len);
		assert(len - strlen(com_cmdline) > 0);
		strncat (com_cmdline, " ", len);
	}
	com_cmdline[len - 1] = '\0';

	if (safe)
	{
	// force all the safe-mode switches. Note that we reserved extra space in
	// case we need to add these, so we don't need an overflow check
		for (i=0 ; i<NUM_SAFE_ARGVS ; i++)
		{
			largv[com_argc] = safeargvs[i];
			com_argc++;
		}
	}

	largv[com_argc] = argvdummy;
	com_argv = largv;

	if (COM_CheckParm ("-rogue"))
	{
		rogue = true;
		standard_quake = false;
	}

	if (COM_CheckParm ("-hipnotic"))
	{
		hipnotic = true;
		standard_quake = false;
	}
}

/*
================
COM_AddParm

Adds the given string at the end of the current argument list
================
*/
void COM_AddParm (char *parm)
{
	largv[com_argc++] = parm;
}
