/*
	gl_rlight.c

	(description)

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
#include <glquake.h>
#include <client.h>
#include <mathlib.h>
#include <view.h>

int	r_dlightframecount;


/*
==================
R_AnimateLight
==================
*/
void
R_AnimateLight ( void )
{
	int			i,j,k;

//
// light animations
// 'm' is normal light, 'a' is no light, 'z' is double bright
	i = (int)(cl.time*10);
	for (j=0 ; j<MAX_LIGHTSTYLES ; j++)
	{
		if (!cl_lightstyle[j].length)
		{
			d_lightstylevalue[j] = 256;
			continue;
		}
		k = i % cl_lightstyle[j].length;
		k = cl_lightstyle[j].map[k] - 'a';
		k = k*22;
		d_lightstylevalue[j] = k;
	}
}

/*
=============================================================================

DYNAMIC LIGHTS BLEND RENDERING

=============================================================================
*/

void
AddLightBlend ( float r, float g, float b, float a2 )
{
	float	a;

	v_blend[3] = a = v_blend[3] + a2*(1-v_blend[3]);

	a2 = a2/a;

	v_blend[0] = v_blend[1]*(1-a2) + r*a2;
	v_blend[1] = v_blend[1]*(1-a2) + g*a2;
	v_blend[2] = v_blend[2]*(1-a2) + b*a2;
//Con_Printf("AddLightBlend(): %4.2f %4.2f %4.2f %4.6f\n", v_blend[0], v_blend[1], v_blend[2], v_blend[3]);
}

float bubble_sintable[17], bubble_costable[17];

void
R_InitBubble ( void )
{
	float a;
	int i;
	float *bub_sin, *bub_cos;

	bub_sin = bubble_sintable;
	bub_cos = bubble_costable;

	for (i=16 ; i>=0 ; i--)
	{
		a = i/16.0 * M_PI*2;
		*bub_sin++ = sin(a);
		*bub_cos++ = cos(a);
	}
}

void
R_RenderDlight ( dlight_t *light )
{
	int		i, j;
	float	a;
	vec3_t	v;
	float	rad;
//	float	*bub_sin, *bub_cos;

	//bub_sin = bubble_sintable;
	//bub_cos = bubble_costable;
	rad = light->radius * 0.35;

	VectorSubtract (light->origin, r_origin, v);
	if (Length (v) < rad)
	{	// view is inside the dlight
		AddLightBlend (1, 0.5, 0, light->radius * 0.0003);
		return;
	}

	glBegin (GL_TRIANGLE_FAN);
	if (light->color[0] || light->color[1] || light->color[2]
			|| light->color[3])	// is there a color?
		glColor4f (light->color[0], light->color[1],
				light->color[2], light->color[3]);
	else
		glColor3f (0.2,0.1,0.05);	// nope, use default
	for (i=0 ; i<3 ; i++)
		v[i] = light->origin[i] - vpn[i]*rad;
	glVertex3fv (v);
	glColor3f (0,0,0);
	for (i=16 ; i>=0 ; i--)
	{
		a = i/16.0 * M_PI*2;
		for (j=0 ; j<3 ; j++)
			v[j] = light->origin[j] + vright[j]*cos(a)*rad + vup[j]*sin(a)*rad;

		/*
		for (j=0 ; j<3 ; j++)
			v[j] = light->origin[j] + (vright[j]*(*bub_cos) +
				+ vup[j]*(*bub_sin)) * rad;
		bub_sin++;
		bub_cos++;
		*/
		glVertex3fv (v);
	}
	glEnd ();
}

/*
=============
R_RenderDlights
=============
*/
void
R_RenderDlights ( void )
{
	int		i;
	dlight_t	*l;

	if (!gl_flashblend->value)
		return;

	r_dlightframecount = r_framecount + 1;	// because the count hasn't

	if (r_fog->value)
		glDisable (GL_FOG);

	// advanced yet for this frame
	glDepthMask (0);
	glDisable (GL_TEXTURE_2D);
	glShadeModel (GL_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);

	l = cl_dlights;
	for (i=0 ; i<MAX_DLIGHTS ; i++, l++)
	{
		if (l->die < cl.time || !l->radius)
			continue;
		R_RenderDlight (l);
	}

	glColor3f (1,1,1);
	glDisable (GL_BLEND);
	glEnable (GL_TEXTURE_2D);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask (1);
	if (r_fog->value)
		glEnable (GL_FOG);
}


/*
=============================================================================

DYNAMIC LIGHTS

=============================================================================
*/

/*
=============
R_MarkLights
=============
*/
void
R_MarkLights ( dlight_t *light, int bit, mnode_t *node )
{
	mplane_t	*splitplane;
	float		dist;
	msurface_t	*surf;
	int			i;

	if (node->contents < 0)
		return;

	splitplane = node->plane;
	dist = DotProduct (light->origin, splitplane->normal) - splitplane->dist;

	if (dist > light->radius)
	{
		R_MarkLights (light, bit, node->children[0]);
		return;
	}
	if (dist < -light->radius)
	{
		R_MarkLights (light, bit, node->children[1]);
		return;
	}

// mark the polygons
	surf = cl.worldmodel->surfaces + node->firstsurface;
	for (i=0 ; i<node->numsurfaces ; i++, surf++)
	{
		if (surf->dlightframe != r_dlightframecount)
		{
			surf->dlightbits = 0;
			surf->dlightframe = r_dlightframecount;
		}
		surf->dlightbits |= bit;
	}

	R_MarkLights (light, bit, node->children[0]);
	R_MarkLights (light, bit, node->children[1]);
}


/*
=============
R_PushDlights
=============
*/
void
R_PushDlights ( void )
{
	int		i;
	dlight_t	*l;

	if (gl_flashblend->value)
		return;

	r_dlightframecount = r_framecount + 1;	// because the count hasn't
											// advanced yet for this frame
	l = cl_dlights;

	for (i=0 ; i<MAX_DLIGHTS ; i++, l++)
	{
		if (l->die < cl.time || !l->radius)
			continue;
		R_MarkLights ( l, 1<<i, cl.worldmodel->nodes );
	}
}


/*
=============================================================================

LIGHT SAMPLING

=============================================================================
*/

mplane_t		*lightplane;
vec3_t			lightspot;
static int		myr[4];
/*
	RecursiveLightPoint
*/
int *
RecursiveLightPoint ( mnode_t *node, vec3_t start, vec3_t end )
{
	int		*r = myr;
	float		front, back, frac;
	int		side;
	mplane_t	*plane;
	vec3_t		mid;
	msurface_t	*surf;
	int		s, t, ds, dt;
	int		i;
	mtexinfo_t	*tex;
	byte		*lightmap;
	unsigned	scale;
	int		maps;

	r[0] = r[1] = r[2] = r[3] = 0;

	if (node->contents < 0)
	{
		r[3] = -1;
		return r;		// didn't hit anything
	}

// calculate mid point

// FIXME: optimize for axial
	plane = node->plane;
	front = DotProduct (start, plane->normal) - plane->dist;
	back = DotProduct (end, plane->normal) - plane->dist;
	side = front < 0;

	if ( (back < 0) == side)
		return RecursiveLightPoint (node->children[side], start, end);

	frac = front / (front-back);
	mid[0] = start[0] + (end[0] - start[0])*frac;
	mid[1] = start[1] + (end[1] - start[1])*frac;
	mid[2] = start[2] + (end[2] - start[2])*frac;

// go down front side
	r = RecursiveLightPoint (node->children[side], start, mid);
	if (r[3] >= 0)
		return r;		// hit something

	if ( (back < 0) == side )
	{
		r[3] = -1;
		return r;		// didn't hit anuthing
	}

// check for impact on this node
	VectorCopy (mid, lightspot);
	lightplane = plane;

	surf = cl.worldmodel->surfaces + node->firstsurface;
	for (i=0 ; i<node->numsurfaces ; i++, surf++)
	{
		if (surf->flags & SURF_DRAWTILED)
			continue;	// no lightmaps

		tex = surf->texinfo;

		s = DotProduct (mid, tex->vecs[0]) + tex->vecs[0][3];
		t = DotProduct (mid, tex->vecs[1]) + tex->vecs[1][3];;

		if (s < surf->texturemins[0] ||
		t < surf->texturemins[1])
			continue;

		ds = s - surf->texturemins[0];
		dt = t - surf->texturemins[1];

		if ( ds > surf->extents[0] || dt > surf->extents[1] )
			continue;

		if (!surf->samples)
			return r;

		ds >>= 4;
		dt >>= 4;

		lightmap = surf->samples;
		if (lightmap)
		{
			if (bspver == CBSPVERSION)
				lightmap += (dt * ((surf->extents[0]>>4)+1)
						+ ds) * 4;
			else
				lightmap += (dt * ((surf->extents[0]>>4)+1)
						+ ds);


			for (maps = 0 ; (maps < MAXLIGHTMAPS)
					&& (surf->styles[maps] != 255) ;
					maps++)
			{
				scale = d_lightstylevalue[surf->styles[maps]];
				if (bspver == CBSPVERSION)
				{
					// calc color's effect for model
					r[0] += lightmap[0] * scale;
					r[1] += lightmap[1] * scale;
					r[2] += lightmap[2] * scale;
					r[3] += lightmap[3] * scale;
					lightmap += (((surf->extents[0]>>4)+1)
							* ((surf->extents[1]
							>>4)+1)) * 4;
				} else {
					r[3] += *lightmap * scale;
					lightmap += ((surf->extents[0]>>4)+1)
						* ((surf->extents[1]>>4)+1);
				}
			}
			if (bspver == CBSPVERSION)
			{
				r[0] = r[1] = r[2] = 0;
			} else {
				r[0] >>= 8;
				r[1] >>= 8;
				r[2] >>= 8;
			}
			r[3] >>= 8;
		}
		return r;
	}

// go down back side
	return RecursiveLightPoint (node->children[!side], mid, end);
}

/*
	R_LightPoint
*/
int *
R_LightPoint ( vec3_t p )
{
	vec3_t		end;
	int		*r = myr;

	if (!cl.worldmodel->lightdata)
	{
		r[0] = r[1] = r[2] = r[3] = 255;
		return r;
	}

	end[0] = p[0];
	end[1] = p[1];
	end[2] = p[2] - 2048;

	r = RecursiveLightPoint (cl.worldmodel->nodes, p, end);

	if (r[3] == -1)
		r[3] = 0;

	return r;
}
