/*
	quakefs.h

	quake virtual filesystem definitions

	Copyright (C) 1996-1997  Id Software, Inc.
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

#ifndef _QUAKEFS_H
#define _QUAKEFS_H

#include <config.h>
#include <quakeio.h>

//============================================================================

#define	MAX_OSPATH	128		// max length of a filesystem pathname

extern int com_filesize;
struct cache_user_s;

extern char	com_gamedir[MAX_OSPATH];
extern char	gamedirfile[MAX_OSPATH];

void COM_WriteFile (char *filename, void *data, int len);
int COM_FOpenFile (char *filename, QFile **gzfile);
void COM_CloseFile (QFile *h);
int COM_filelength (QFile *f);

byte *COM_LoadStackFile (char *path, void *buffer, int bufsize);
byte *COM_LoadTempFile (char *path);
byte *COM_LoadHunkFile (char *path);
void COM_LoadCacheFile (char *path, struct cache_user_s *cu);
void COM_CreatePath (char *path);
void COM_Gamedir (char *dir);
void COM_InitFilesystem (void);
void COM_Path_f (void);
void COM_Maplist_f (void);

#endif // _QUAKEFS_H
