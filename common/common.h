/*
	common.h

	general definitions

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

#ifndef _COMMON_H
#define _COMMON_H

//#ifdef QUAKEWORLD
//#	include <bothdefs.h>
//#endif
#include <register_check.h>
#include <qtypes.h>
#include <cvar.h>

#define	MAX_INFO_STRING		512
#define	MAX_SERVERINFO_STRING	512
#define	MAX_LOCALINFO_STRING	32768

//============================================================================

typedef struct sizebuf_s
{
	qboolean	allowoverflow;	// if false, do a Sys_Error
	qboolean	overflowed;		// set to true if the buffer size failed
	byte	*data;
	int		maxsize;
	int		cursize;
} sizebuf_t;

#include <net.h>
void SZ_Alloc (sizebuf_t *buf, int startsize);
void SZ_Free (sizebuf_t *buf);
void SZ_Clear (sizebuf_t *buf);
void *SZ_GetSpace (sizebuf_t *buf, int length);
void SZ_Write (sizebuf_t *buf, void *data, int length);
void SZ_Print (sizebuf_t *buf, char *data);	// strcats onto the sizebuf

//============================================================================

typedef struct link_s
{
	struct link_s	*prev, *next;
} link_t;


void ClearLink (link_t *l);
void RemoveLink (link_t *l);
void InsertLinkBefore (link_t *l, link_t *before);
void InsertLinkAfter (link_t *l, link_t *after);

// (type *)STRUCT_FROM_LINK(link_t *link, type, member)
// ent = STRUCT_FROM_LINK(link,entity_t,order)
// FIXME: remove this mess!
#define	STRUCT_FROM_LINK(l,t,m) ((t *)((byte *)l - (int)&(((t *)0)->m)))

//============================================================================
// Pak 3 loading support:
/*
pack_t *COM_LoadQ3PackFile (char *packfile);
uint_t COM_pak3_getlen(unzFile *pak);
int COM_pak3_open(unzFile *pak, const char *path);
uint_t COM_pak3_readfile(unzFile *pak, const char *path, uint_t bufsize, byte_t *buf);
int COM_pak3_read(unzFile *pak, void *buf, uint_t size, uint_t nmemb);
int COM_pak3_checkfile(unzFile *pak, const char *path);
*/


// End Pak3 support.
//============================================================================

#ifndef NULL
#define NULL ((void *)0)
#endif

#define Q_MAXCHAR ((char)0x7f)
#define Q_MAXSHORT ((short)0x7fff)
#define Q_MAXINT	((int)0x7fffffff)
#define Q_MAXLONG ((int)0x7fffffff)
#define Q_MAXFLOAT ((int)0x7fffffff)

#define Q_MINCHAR ((char)0x80)
#define Q_MINSHORT ((short)0x8000)
#define Q_MININT 	((int)0x80000000)
#define Q_MINLONG ((int)0x80000000)
#define Q_MINFLOAT ((int)0x7fffffff)

//============================================================================

#include <qendian.h>

//============================================================================

struct usercmd_s;

extern struct usercmd_s nullcmd;

void MSG_WriteChar (sizebuf_t *sb, int c);
void MSG_WriteByte (sizebuf_t *sb, int c);
void MSG_WriteShort (sizebuf_t *sb, int c);
void MSG_WriteLong (sizebuf_t *sb, int c);
void MSG_WriteFloat (sizebuf_t *sb, float f);
void MSG_WriteString (sizebuf_t *sb, char *s);
void MSG_WriteCoord (sizebuf_t *sb, float f);
void MSG_WriteAngle (sizebuf_t *sb, float f);
void MSG_WriteAngle16 (sizebuf_t *sb, float f);
void MSG_WriteDeltaUsercmd (sizebuf_t *sb, struct usercmd_s *from, struct usercmd_s *cmd);

extern	int			msg_readcount;
extern	qboolean	msg_badread;		// set if a read goes beyond end of message

void MSG_BeginReading (void);
int MSG_GetReadCount(void);
int MSG_ReadChar (void);
int MSG_ReadByte (void);
int MSG_ReadShort (void);
int MSG_ReadLong (void);
float MSG_ReadFloat (void);
char *MSG_ReadString (void);
char *MSG_ReadStringLine (void);

float MSG_ReadCoord (void);
float MSG_ReadAngle (void);
float MSG_ReadAngle16 (void);
void MSG_ReadDeltaUsercmd (struct usercmd_s *from, struct usercmd_s *cmd);

//============================================================================

#include <lib_replace.h>

//============================================================================

extern	int			com_no_escapes;
extern	char		com_token[1024];
extern	qboolean	com_eof;

char *COM_Parse (char *data);
char *COM_EscapeEscapes(char *str);


extern	int		com_argc;
extern	char	**com_argv;
extern	cvar_t	*cmdline;

int COM_CheckParm (char *parm);
void COM_AddParm (char *parm);
void COM_Init (void);
void COM_InitArgv (int argc, char **argv);

char *COM_SkipPath (char *pathname);
void COM_StripExtension (char *in, char *out);
void COM_FileBase (char *in, char *out);
void COM_DefaultExtension (char *path, char *extension);

#ifdef __GNUC__
char	*va(char *format, ...) __attribute__((format (printf, 1, 2)));
#else
char	*va(char *format, ...);
#endif
// does a varargs printf into a temp buffer


//============================================================================

#include <quakefs.h>

extern qboolean		standard_quake, rogue, hipnotic;
byte *COM_LoadStackFile (char *path, void *buffer, int bufsize);
byte *COM_LoadTempFile (char *path);
byte *COM_LoadHunkFile (char *path);
void COM_LoadCacheFile (char *path, struct cache_user_s *cu);

char *Info_ValueForKey (char *s, char *key);
void Info_RemoveKey (char *s, char *key);
void Info_RemovePrefixedKeys (char *start, char prefix);
void Info_SetValueForKey (char *s, char *key, char *value, int maxsize);
void Info_SetValueForStarKey (char *s, char *key, char *value, int maxsize);
void Info_Print (char *s);

unsigned Com_BlockChecksum (void *buffer, int length);
void Com_BlockFullChecksum (void *buffer, int len, unsigned char *outbuf);
byte	COM_BlockSequenceCheckByte (byte *base, int length, int sequence, unsigned mapchecksum);
byte	COM_BlockSequenceCRCByte (byte *base, int length, int sequence);

#endif // _COMMON_H
