/*
	cl_tent.c

	client side temporary entities

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

#include <qtypes.h>
#include <quakedef.h>
#include <model.h>
#include <sound.h>
#include <client.h>
#include <mathlib.h>
#include <console.h>

#define	MAX_BEAMS	32
typedef struct
{
	int		entity;
	struct model_s	*model;
	float		endtime;
	vec3_t		start, end;
} beam_t;

#define	MAX_EXPLOSIONS	32
typedef struct
{
	vec3_t		origin;
	float		start;
	model_t		*model;
} explosion_t;

explosion_t	cl_explosions[MAX_EXPLOSIONS];
beam_t		cl_beams[MAX_BEAMS];

// UQ used these
// int			num_temp_entities;
// entity_t	cl_temp_entities[MAX_TEMP_ENTITIES];

sfx_t			*cl_sfx_wizhit;
sfx_t			*cl_sfx_knighthit;
sfx_t			*cl_sfx_tink1;
sfx_t			*cl_sfx_ric1;
sfx_t			*cl_sfx_ric2;
sfx_t			*cl_sfx_ric3;
sfx_t			*cl_sfx_r_exp3;
#ifdef QUAKE2
sfx_t			*cl_sfx_imp;
sfx_t			*cl_sfx_rail;
#endif


/*
    CL_InitTEnts
*/
void
CL_InitTEnts ( void )
{
	cl_sfx_wizhit = S_PrecacheSound ("wizard/hit.wav");
	cl_sfx_knighthit = S_PrecacheSound ("hknight/hit.wav");
	cl_sfx_tink1 = S_PrecacheSound ("weapons/tink1.wav");
	cl_sfx_ric1 = S_PrecacheSound ("weapons/ric1.wav");
	cl_sfx_ric2 = S_PrecacheSound ("weapons/ric2.wav");
	cl_sfx_ric3 = S_PrecacheSound ("weapons/ric3.wav");
	cl_sfx_r_exp3 = S_PrecacheSound ("weapons/r_exp3.wav");
#ifdef QUAKE2
	cl_sfx_imp = S_PrecacheSound ("shambler/sattck1.wav");
	cl_sfx_rail = S_PrecacheSound ("weapons/lstart.wav");
#endif
}


/*
    CL_ClearTEnts
*/
void
CL_ClearTEnts ( void )
{
	memset (cl_beams, 0, sizeof(cl_beams));
	memset (cl_explosions, 0, sizeof(cl_explosions));
}


/*
    CL_AllocExplosion
*/
explosion_t *
CL_AllocExplosion ( void )
{
	int		i;
	float		time;
	int		index;

	for (i=0 ; i<MAX_EXPLOSIONS ; i++)
		if (!cl_explosions[i].model)
			return &cl_explosions[i];
// find the oldest explosion
	time = cl.time;
	index = 0;

	for (i=0 ; i<MAX_EXPLOSIONS ; i++)
		if (cl_explosions[i].start < time) {
			time = cl_explosions[i].start;
			index = i;
		}
	return &cl_explosions[index];
}


/*
    CL_ParseBeam
*/
void
CL_ParseBeam ( model_t *m )
{
	int		ent;
	vec3_t		start, end;
	beam_t		*b;
	int		i;

	ent = MSG_ReadShort ();

	start[0] = MSG_ReadCoord ();
	start[1] = MSG_ReadCoord ();
	start[2] = MSG_ReadCoord ();

	end[0] = MSG_ReadCoord ();
	end[1] = MSG_ReadCoord ();
	end[2] = MSG_ReadCoord ();

// override any beam with the same entity
	for (i=0, b=cl_beams ; i< MAX_BEAMS ; i++, b++)
		if (b->entity == ent) {
			b->entity = ent;
			b->model = m;
			b->endtime = cl.time + 0.2;
			VectorCopy (start, b->start);
			VectorCopy (end, b->end);
			return;
		}

// find a free beam
	for (i=0, b=cl_beams ; i< MAX_BEAMS ; i++, b++) {
		if (!b->model || b->endtime < cl.time) {
			b->entity = ent;
			b->model = m;
			b->endtime = cl.time + 0.2;
			VectorCopy (start, b->start);
			VectorCopy (end, b->end);
			return;
		}
	}
	Con_Printf ("beam list overflow!\n");
}


/*
    CL_ParseTEnt
*/
void
CL_ParseTEnt ( void )
{
	int		type;
	vec3_t		pos;
#ifdef QUAKE2
	vec3_t		endpos;
#endif
	dlight_t	*dl;
	int		rnd;
#ifdef UQUAKE
	int		colorStart, colorLength;
#endif
	explosion_t	*ex;
#ifdef QUAKEWORLD
	int		cnt;
#endif

	type = MSG_ReadByte ();
	switch (type)
	{
	case TE_WIZSPIKE:			// spike hitting wall
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_RunParticleEffect (pos, vec3_origin, 20, 30);
		S_StartSound (-1, 0, cl_sfx_wizhit, pos, 1, 1);
		break;

	case TE_KNIGHTSPIKE:			// spike hitting wall
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_RunParticleEffect (pos, vec3_origin, 226, 20);
		S_StartSound (-1, 0, cl_sfx_knighthit, pos, 1, 1);
		break;

	case TE_SPIKE:			// spike hitting wall
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
#ifdef GLTEST
		Test_Spawn (pos);
#else
		R_RunParticleEffect (pos, vec3_origin, 0, 10);
#endif
		if ( rand() % 5 )
			S_StartSound (-1, 0, cl_sfx_tink1, pos, 1, 1);
		else {
			rnd = rand() & 3;
			if (rnd == 1)
				S_StartSound (-1, 0, cl_sfx_ric1, pos, 1, 1);
			else if (rnd == 2)
				S_StartSound (-1, 0, cl_sfx_ric2, pos, 1, 1);
			else
				S_StartSound (-1, 0, cl_sfx_ric3, pos, 1, 1);
		}
		break;

	case TE_SUPERSPIKE:			// super spike hitting wall
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_RunParticleEffect (pos, vec3_origin, 0, 20);

		if ( rand() % 5 )
			S_StartSound (-1, 0, cl_sfx_tink1, pos, 1, 1);
		else {
			rnd = rand() & 3;
			if (rnd == 1)
				S_StartSound (-1, 0, cl_sfx_ric1, pos, 1, 1);
			else if (rnd == 2)
				S_StartSound (-1, 0, cl_sfx_ric2, pos, 1, 1);
			else
				S_StartSound (-1, 0, cl_sfx_ric3, pos, 1, 1);
		}
		break;

	case TE_EXPLOSION:			// rocket explosion
	// particles
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_ParticleExplosion (pos);

	// light
		dl = CL_AllocDlight (0);
		VectorCopy (pos, dl->origin);
		dl->radius = 350;
		dl->die = cl.time + 0.5;
		dl->decay = 300;
		dl->color[0] = 0.86;
		dl->color[1] = 0.31;
		dl->color[2] = 0.24;
		dl->color[3] = 0.7;

	// sound
		S_StartSound (-1, 0, cl_sfx_r_exp3, pos, 1, 1);

	// sprite
		ex = CL_AllocExplosion ();
		VectorCopy (pos, ex->origin);
		ex->start = cl.time;
		ex->model = Mod_ForName ("progs/s_explod.spr", true);
		break;


	case TE_TAREXPLOSION:			// tarbaby explosion
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_BlobExplosion (pos);

		S_StartSound (-1, 0, cl_sfx_r_exp3, pos, 1, 1);
		break;

	case TE_LIGHTNING1:				// lightning bolts
		CL_ParseBeam (Mod_ForName("progs/bolt.mdl", true));
		break;

	case TE_LIGHTNING2:				// lightning bolts
		CL_ParseBeam (Mod_ForName("progs/bolt2.mdl", true));
		break;

	case TE_LIGHTNING3:				// lightning bolts
		CL_ParseBeam (Mod_ForName("progs/bolt3.mdl", true));
		break;

	case TE_LAVASPLASH:
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_LavaSplash (pos);
		break;

	case TE_TELEPORT:
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_TeleportSplash (pos);
		break;

#ifdef QUAKEWORLD
	case TE_GUNSHOT:			// bullet hitting wall
		cnt = MSG_ReadByte ();
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_RunParticleEffect (pos, vec3_origin, 0, 20 * cnt);
		break;

	case TE_BLOOD:				// bullets hitting body
		cnt = MSG_ReadByte ();
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_RunParticleEffect (pos, vec3_origin, 73, 20*cnt);
		break;

	case TE_LIGHTNINGBLOOD:		// lightning hitting body
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_RunParticleEffect (pos, vec3_origin, 225, 50);
		break;
#endif	// QUAKEWORLD

#ifdef UQUAKE
	case TE_GUNSHOT:			// bullet hitting wall
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		R_RunParticleEffect (pos, vec3_origin, 0, 20);
		break;

// PGM 01/21/97
	case TE_BEAM:				// grappling hook beam
		CL_ParseBeam (Mod_ForName("progs/beam.mdl", true));
		break;
// PGM 01/21/97

	case TE_EXPLOSION2:		// color mapped explosion
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		colorStart = MSG_ReadByte ();
		colorLength = MSG_ReadByte ();
		R_ParticleExplosion2 (pos, colorStart, colorLength);
		dl = CL_AllocDlight (0);
		VectorCopy (pos, dl->origin);
		dl->radius = 350;
		dl->die = cl.time + 0.5;
		dl->decay = 300;
		S_StartSound (-1, 0, cl_sfx_r_exp3, pos, 1, 1);
		break;
#endif	// UQUAKE

#ifdef QUAKE2
	case TE_IMPLOSION:
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		S_StartSound (-1, 0, cl_sfx_imp, pos, 1, 1);
		break;

	case TE_RAILTRAIL:
		pos[0] = MSG_ReadCoord ();
		pos[1] = MSG_ReadCoord ();
		pos[2] = MSG_ReadCoord ();
		endpos[0] = MSG_ReadCoord ();
		endpos[1] = MSG_ReadCoord ();
		endpos[2] = MSG_ReadCoord ();
		S_StartSound (-1, 0, cl_sfx_rail, pos, 1, 1);
		S_StartSound (-1, 1, cl_sfx_r_exp3, endpos, 1, 1);
		R_RocketTrail (pos, endpos, 0+128);
		R_ParticleExplosion (endpos);
		dl = CL_AllocDlight (-1);
		VectorCopy (endpos, dl->origin);
		dl->radius = 350;
		dl->die = cl.time + 0.5;
		dl->decay = 300;
		break;
#endif

	default:
		Sys_Error ("CL_ParseTEnt: bad type");
	}
}


/*
    CL_NewTempEntity
*/
entity_t *
CL_NewTempEntity ( void )
{
	entity_t	*ent;

	if (cl_numvisedicts == MAX_VISEDICTS)
		return NULL;

	ent = &cl_visedicts[cl_numvisedicts];
	cl_numvisedicts++;

	memset (ent, 0, sizeof(*ent));

	ent->colormap = vid.colormap;
	return ent;
}


/*
    CL_UpdateBeams
*/
void
CL_UpdateBeams ( void )
{
	int			i,j;
	beam_t		*b;
	vec3_t		dist, org;
	float		d;
	entity_t	*ent;
	float		yaw, pitch;
	float		forward;

// update lightning
	for (i=0, b=cl_beams ; i< MAX_BEAMS ; i++, b++)
	{
		if (!b->model || b->endtime < cl.time)
			continue;

	// if coming from the player, update the start position
		if (b->entity == cl.playernum + 1) {
#ifdef QUAKEWORLD
			VectorCopy (cl.simorg, b->start);
#elif UQUAKE
			VectorCopy (cl_entities[cl.playernum + 1].origin,
					b->start);
#endif
		}

	// calculate pitch and yaw
		VectorSubtract (b->end, b->start, dist);

		if (dist[1] == 0 && dist[0] == 0) {
			yaw = 0;
			if (dist[2] > 0)
				pitch = 90;
			else
				pitch = 270;
		} else {
			yaw = (int) (atan2(dist[1], dist[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			forward = sqrt (dist[0]*dist[0] + dist[1]*dist[1]);
			pitch = (int) (atan2(dist[2], forward) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

	// add new entities for the lightning
		VectorCopy (b->start, org);
		d = VectorNormalize(dist);
		while (d > 0) {
			ent = CL_NewTempEntity ();
			if (!ent)
				return;
			VectorCopy (org, ent->origin);
			ent->model = b->model;
			ent->angles[0] = pitch;
			ent->angles[1] = yaw;
			ent->angles[2] = rand()%360;

			for (j=0 ; j<3 ; j++)
				org[j] += dist[j]*30;
			d -= 30;
		}
	}
}


/*
    CL_UpdateExplosions
*/
void
CL_UpdateExplosions ( void )
{
	int			i;
	int			f;
	explosion_t	*ex;
	entity_t	*ent;

	for (i=0, ex=cl_explosions ; i< MAX_EXPLOSIONS ; i++, ex++)
	{
		if (!ex->model)
			continue;
		f = 10*(cl.time - ex->start);
		if (f >= ex->model->numframes)
		{
			ex->model = NULL;
			continue;
		}

		ent = CL_NewTempEntity ();
		if (!ent)
			return;
		VectorCopy (ex->origin, ent->origin);
		ent->model = ex->model;
		ent->frame = f;
	}
}


/*
    CL_UpdateTEnts
*/
void
CL_UpdateTEnts ( void )
{
	CL_UpdateBeams ();
	CL_UpdateExplosions ();
}
