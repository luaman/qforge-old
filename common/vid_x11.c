/*
	vid_x11.c

	general x11 video driver

	Copyright (C) 1996-1997  Id Software, Inc.
	Copyright (C) 1999-2000  contributors of the QuakeForge project
	Copyright (C) 2000       Marcus Sundberg [mackan@stacken.kth.se]
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

#define _BSD
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/XShm.h>

#include <quakedef.h>
#include <d_local.h>
#include <keys.h>
#include <cvar.h>
#include <menu.h>
#include <sys.h>
#include <cmd.h>
#include <lib_replace.h>
#include <draw.h>
#include <console.h>
#include <client.h>
#include <input.h>
#include <context_x11.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

extern viddef_t        vid; // global video state
unsigned short	d_8to16table[256];

Window				x_win;
static Colormap		x_cmap;
static GC			x_gc;
static Visual		*x_vis;
static XVisualInfo	*x_visinfo;
static Atom			aWMDelete = 0;


int XShmQueryExtension(Display *);
int XShmGetEventBase(Display *);

qboolean				doShm;
static XShmSegmentInfo	x_shminfo[2];

static int			current_framebuffer;
static XImage		*x_framebuffer[2] = { 0, 0 };

static int verbose = 0;

int	VID_options_items = 1;

static byte current_palette[768];

typedef unsigned short PIXEL16;
typedef unsigned long PIXEL24;
static PIXEL16 st2d_8to16table[256];
static PIXEL24 st2d_8to24table[256];
static int shiftmask_fl=0;
static long r_shift,g_shift,b_shift;
static unsigned long r_mask,g_mask,b_mask;

static long X11_highhunkmark;

#define STD_EVENT_MASK \
			( VisibilityChangeMask | ExposureMask | StructureNotifyMask)


static void
shiftmask_init()
{
	unsigned int x;

	r_mask=x_vis->red_mask;
	g_mask=x_vis->green_mask;
	b_mask=x_vis->blue_mask;
	for (r_shift=-8,x=1; x<r_mask; x <<= 1) r_shift++;
	for (g_shift=-8,x=1; x<g_mask; x <<= 1) g_shift++;
	for (b_shift=-8,x=1; x<b_mask; x <<= 1) b_shift++;
	shiftmask_fl=1;
}


static PIXEL16
xlib_rgb16(int r, int g, int b)
{
	PIXEL16 p = 0;

	if (shiftmask_fl==0) shiftmask_init();

	if (r_shift>0) {
		p=(r<<(r_shift))&r_mask;
	} else if (r_shift<0) {
		p=(r>>(-r_shift))&r_mask;
	} else p|=(r&r_mask);

	if (g_shift>0) {
		p|=(g<<(g_shift))&g_mask;
	} else if (g_shift<0) {
		p|=(g>>(-g_shift))&g_mask;
	} else p|=(g&g_mask);

	if (b_shift>0) {
		p|=(b<<(b_shift))&b_mask;
	} else if (b_shift<0) {
		p|=(b>>(-b_shift))&b_mask;
	} else p|=(b&b_mask);

	return p;
}


static PIXEL24
xlib_rgb24(int r,int g,int b)
{
	PIXEL24 p = 0;

	if (shiftmask_fl==0) shiftmask_init();

	if (r_shift>0) {
		p=(r<<(r_shift))&r_mask;
	} else if (r_shift<0) {
		p=(r>>(-r_shift))&r_mask;
	} else p|=(r&r_mask);

	if (g_shift>0) {
		p|=(g<<(g_shift))&g_mask;
	} else if (g_shift<0) {
		p|=(g>>(-g_shift))&g_mask;
	} else p|=(g&g_mask);

	if (b_shift>0) {
		p|=(b<<(b_shift))&b_mask;
	} else if (b_shift<0) {
		p|=(b>>(-b_shift))&b_mask;
	} else p|=(b&b_mask);

	return p;
}


static void
st2_fixup(XImage *framebuf, int x, int y, int width, int height)
{
	int xi,yi;
	unsigned char *src;
	PIXEL16 *dest;

	if (x < 0 || y < 0) return;

	for (yi = y; yi < (y+height); yi++) {
		src = &framebuf->data [yi * framebuf->bytes_per_line];
		dest = (PIXEL16*)src;
		for(xi = (x+width-1); xi >= x; xi--) {
			dest[xi] = st2d_8to16table[src[xi]];
		}
	}
}


static void
st3_fixup(XImage *framebuf, int x, int y, int width, int height)
{
	int yi;
	unsigned char *src;
	PIXEL24 *dest;
	register int count, n;

	if (x < 0 || y < 0) return;

	for (yi = y; yi < (y+height); yi++) {
		src = &framebuf->data [yi * framebuf->bytes_per_line];

		// Duff's Device
		count = width;
		n = (count + 7) / 8;
		dest = ((PIXEL24 *)src) + x+width - 1;
		src += x+width - 1;

		switch (count % 8) {
		case 0:	do {	*dest-- = st2d_8to24table[*src--];
		case 7:			*dest-- = st2d_8to24table[*src--];
		case 6:			*dest-- = st2d_8to24table[*src--];
		case 5:			*dest-- = st2d_8to24table[*src--];
		case 4:			*dest-- = st2d_8to24table[*src--];
		case 3:			*dest-- = st2d_8to24table[*src--];
		case 2:			*dest-- = st2d_8to24table[*src--];
		case 1:			*dest-- = st2d_8to24table[*src--];
				} while (--n > 0);
		}

//		for(xi = (x+width-1); xi >= x; xi--) {
//			dest[xi] = st2d_8to16table[src[xi]];
//		}
	}
}

/*
================
D_BeginDirectRect
================
*/
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height)
{
// direct drawing of the "accessing disk" icon isn't supported
}


/*
================
D_EndDirectRect
================
*/
void D_EndDirectRect (int x, int y, int width, int height)
{
// direct drawing of the "accessing disk" icon isn't supported
}


/*
=================
VID_Gamma_f

Keybinding command
=================
*/

byte vid_gamma[256];

void VID_Gamma_f (void)
{

	float	g, f, inf;
	int		i;

	if (Cmd_Argc () == 2) {
		g = Q_atof (Cmd_Argv(1));

		for (i=0 ; i<255 ; i++)	{
			f = pow ((i+1)/256.0, g);
			inf = f*255 + 0.5;
			if (inf < 0) inf = 0;
			if (inf > 255) inf = 255;
			vid_gamma[i] = inf;
		}

		VID_SetPalette(current_palette);

		vid.recalc_refdef = 1;	// force a surface cache flush
	}
}


static void
ResetFrameBuffer(void)
{
	int vid_surfcachesize, buffersize;
	void *vid_surfcache;
	int mem, pwidth;

	if (x_framebuffer[0]) {
		XDestroyImage(x_framebuffer[0]);
	}

	if (d_pzbuffer) {
		D_FlushCaches ();
		Hunk_FreeToHighMark(X11_highhunkmark);
		d_pzbuffer = NULL;
	}
	X11_highhunkmark = Hunk_HighMark ();

	/* Alloc an extra line in case we want to wrap, and allocate
	   the z-buffer */
	buffersize = vid.width * vid.height * sizeof(*d_pzbuffer);

	vid_surfcachesize = D_SurfaceCacheForRes(vid.width, vid.height);

	buffersize += vid_surfcachesize;

	d_pzbuffer = Hunk_HighAllocName(buffersize, "video");
	if (d_pzbuffer == NULL) {
		Sys_Error ("Not enough memory for video mode\n");
	}

	vid_surfcache = (byte *) d_pzbuffer
		+ vid.width * vid.height * sizeof(*d_pzbuffer);

	D_InitCaches(vid_surfcache, vid_surfcachesize);

	pwidth = x_visinfo->depth / 8;
	if (pwidth == 3) pwidth = 4;
	mem = ((vid.width*pwidth+7)&~7) * vid.height;

	x_framebuffer[0] = XCreateImage(x_disp, x_vis, x_visinfo->depth,
					ZPixmap, 0,
					malloc(mem),
					vid.width, vid.height,
					32, 0);

	if (!x_framebuffer[0]) {
		Sys_Error("VID: XCreateImage failed\n");
	}
}


static void
ResetSharedFrameBuffers(void)
{
	int vid_surfcachesize, buffersize;
	void *vid_surfcache;
	int size;
	int key;
	int minsize = getpagesize();
	int frm;

	if (d_pzbuffer)	{
		D_FlushCaches ();
		Hunk_FreeToHighMark(X11_highhunkmark);
		d_pzbuffer = NULL;
	}

	X11_highhunkmark = Hunk_HighMark ();

// alloc an extra line in case we want to wrap, and allocate the z-buffer
	buffersize = vid.width * vid.height * sizeof (*d_pzbuffer);

	vid_surfcachesize = D_SurfaceCacheForRes (vid.width, vid.height);

	buffersize += vid_surfcachesize;

	d_pzbuffer = Hunk_HighAllocName(buffersize, "video");
	if (d_pzbuffer == NULL) {
		Sys_Error ("Not enough memory for video mode\n");
	}

	vid_surfcache = (byte *) d_pzbuffer
		+ vid.width * vid.height * sizeof (*d_pzbuffer);

	D_InitCaches(vid_surfcache, vid_surfcachesize);

	for (frm=0 ; frm<2 ; frm++)
	{

	// free up old frame buffer memory

		if (x_framebuffer[frm])
		{
			XShmDetach(x_disp, &x_shminfo[frm]);
			free(x_framebuffer[frm]);
			shmdt(x_shminfo[frm].shmaddr);
		}

	// create the image

		x_framebuffer[frm] = XShmCreateImage(	x_disp,
						x_vis,
						x_visinfo->depth,
						ZPixmap,
						0,
						&x_shminfo[frm],
						vid.width,
						vid.height );

	// grab shared memory

		size = x_framebuffer[frm]->bytes_per_line
			* x_framebuffer[frm]->height;
		if (size < minsize)
			Sys_Error("VID: Window must use at least %d bytes\n", minsize);

		key = random();
		x_shminfo[frm].shmid = shmget((key_t)key, size, IPC_CREAT|0777);
		if (x_shminfo[frm].shmid==-1)
			Sys_Error("VID: Could not get any shared memory\n");

		// attach to the shared memory segment
		x_shminfo[frm].shmaddr =
			(void *) shmat(x_shminfo[frm].shmid, 0, 0);

		printf("VID: shared memory id=%d, addr=0x%lx\n", x_shminfo[frm].shmid,
			(long) x_shminfo[frm].shmaddr);

		x_framebuffer[frm]->data = x_shminfo[frm].shmaddr;

	// get the X server to attach to it

		if (!XShmAttach(x_disp, &x_shminfo[frm]))
			Sys_Error("VID: XShmAttach() failed\n");
		XSync(x_disp, 0);
		shmctl(x_shminfo[frm].shmid, IPC_RMID, 0);

	}

}

static void event_shm(XEvent *event)
{
    if (doShm)
		oktodraw = true;
}

// Called at startup to set up translation tables, takes 256 8 bit RGB values
// the palette data will go away after the call, so it must be copied off if
// the video driver will need it again

void VID_Init (unsigned char *palette)
{
	int pnum, i;
	XVisualInfo template;
	int num_visuals;
	int template_mask;

	//plugin_load("in_x11.so");

//	Cmd_AddCommand("gamma", VID_Gamma_f);
	for (i=0; i < 256; i++)	vid_gamma[i] = i;

	vid.width = 320;
	vid.height = 200;
	vid.maxwarpwidth = WARP_WIDTH;
	vid.maxwarpheight = WARP_HEIGHT;
	vid.numpages = 2;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
	//vid.cbits = VID_CBITS;
	//vid.grades = VID_GRADES;

	srandom(getpid());

	verbose=COM_CheckParm("-verbose");

// open the display
	x11_open_display();

// check for command-line window size
	if ((pnum=COM_CheckParm("-winsize")))
	{
		if (pnum >= com_argc-2)
			Sys_Error("VID: -winsize <width> <height>\n");
		vid.width = Q_atoi(com_argv[pnum+1]);
		vid.height = Q_atoi(com_argv[pnum+2]);
		if (!vid.width || !vid.height)
			Sys_Error("VID: Bad window width/height\n");
	}
	if ((pnum=COM_CheckParm("-width"))) {
		if (pnum >= com_argc-1)
			Sys_Error("VID: -width <width>\n");
		vid.width = Q_atoi(com_argv[pnum+1]);
		if (!vid.width)
			Sys_Error("VID: Bad window width\n");
	}
	if ((pnum=COM_CheckParm("-height"))) {
		if (pnum >= com_argc-1)
			Sys_Error("VID: -height <height>\n");
		vid.height = Q_atoi(com_argv[pnum+1]);
		if (!vid.height)
			Sys_Error("VID: Bad window height\n");
	}

	template_mask = 0;

// specify a visual id
	if ((pnum=COM_CheckParm("-visualid")))
	{
		if (pnum >= com_argc-1)
			Sys_Error("VID: -visualid <id#>\n");
		template.visualid = Q_atoi(com_argv[pnum+1]);
		template_mask = VisualIDMask;
	}

// If not specified, use default visual
	else
	{
		int screen;
		screen = XDefaultScreen(x_disp);
		template.visualid =
			XVisualIDFromVisual(XDefaultVisual(x_disp, screen));
		template_mask = VisualIDMask;
	}

// pick a visual- warn if more than one was available
	x_visinfo = XGetVisualInfo(x_disp, template_mask, &template, &num_visuals);
	if (num_visuals > 1) {
		printf("Found more than one visual id at depth %d:\n", template.depth);
		for (i=0 ; i<num_visuals ; i++)
			printf("	-visualid %d\n", (int)(x_visinfo[i].visualid));
	} else if (num_visuals == 0) {
		if (template_mask == VisualIDMask) {
			Sys_Error("VID: Bad visual id %d\n",
				  template.visualid);
		} else {
			Sys_Error("VID: No visuals at depth %d\n",
				  template.depth);
		}
	}

	if (verbose) {
		printf("Using visualid %d:\n", (int)(x_visinfo->visualid));
		printf("	class %d\n", x_visinfo->class);
		printf("	screen %d\n", x_visinfo->screen);
		printf("	depth %d\n", x_visinfo->depth);
		printf("	red_mask 0x%x\n", (int)(x_visinfo->red_mask));
		printf("	green_mask 0x%x\n", (int)(x_visinfo->green_mask));
		printf("	blue_mask 0x%x\n", (int)(x_visinfo->blue_mask));
		printf("	colormap_size %d\n", x_visinfo->colormap_size);
		printf("	bits_per_rgb %d\n", x_visinfo->bits_per_rgb);
	}

	x_vis = x_visinfo->visual;

	/* Setup attributes for main window */
	{
		int attribmask = CWEventMask | CWBorderPixel;
		XSetWindowAttributes attribs;

		attribs.event_mask = STD_EVENT_MASK;
		attribs.border_pixel = 0;

		/* Create the main window */
		x_win = XCreateWindow(x_disp,
			      XRootWindow(x_disp, x_visinfo->screen),
			      0, 0, vid.width, vid.height,
			      0, /* borderwidth	*/
			      x_visinfo->depth, InputOutput, x_vis,
			      attribmask, &attribs);

		/* Give it a title */
		XStoreName(x_disp, x_win, "XQuake");

		/* Make window respond to Delete events */
		aWMDelete = XInternAtom(x_disp, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(x_disp, x_win, &aWMDelete, 1);
	}

	if (x_visinfo->depth == 8) {
		/* Create and upload the palette */
		if (x_visinfo->class == PseudoColor) {
			x_cmap = XCreateColormap(x_disp, x_win,
						 x_vis, AllocAll);
			VID_SetPalette(palette);
			XSetWindowColormap(x_disp, x_win, x_cmap);
		}
	}

// create the GC
	{
		XGCValues xgcvalues;
		int valuemask = GCGraphicsExposures;
		xgcvalues.graphics_exposures = False;
		x_gc = XCreateGC(x_disp, x_win, valuemask, &xgcvalues );
	}

// map the window
	XMapWindow(x_disp, x_win);

// wait for first exposure event
	{
		XEvent event;
		do
		{
			XNextEvent(x_disp, &event);
			if (event.type == Expose && !event.xexpose.count)
				oktodraw = true;
		} while (!oktodraw);
	}
// now safe to draw

// even if MITSHM is available, make sure it's a local connection
	if (XShmQueryExtension(x_disp))
	//if (0)
	{
		char *displayname;
		doShm = true;
		displayname = (char *) getenv("DISPLAY");
		if (displayname)
		{
			char *d = displayname;
			while (*d && (*d != ':')) d++;
			if (*d) *d = 0;
			if (!(!strcasecmp(displayname, "unix") || !*displayname))
				doShm = false;
		}
	}

	if (doShm)
	{
		x_shmeventtype = XShmGetEventBase(x_disp) + ShmCompletion;
		ResetSharedFrameBuffers();
	}
	else
		ResetFrameBuffer();

	current_framebuffer = 0;
	vid.rowbytes = x_framebuffer[0]->bytes_per_line;
	vid.buffer = x_framebuffer[0]->data;
	vid.direct = 0;
	vid.conbuffer = x_framebuffer[0]->data;
	vid.conrowbytes = vid.rowbytes;
	vid.conwidth = vid.width;
	vid.conheight = vid.height;
	vid.aspect = ((float)vid.height / (float)vid.width) * (320.0 / 240.0);

//	XSynchronize(x_disp, False);
	x11_add_event(x_shmeventtype, event_shm);
}


void
VID_ShiftPalette(unsigned char *p)
{
	VID_SetPalette(p);
}


void
VID_SetPalette(unsigned char *palette)
{
	int i;
	XColor colors[256];

	for (i=0;i<256;i++) {
		st2d_8to16table[i] = xlib_rgb16(palette[i*3], palette[i*3+1],
						palette[i*3+2]);
		st2d_8to24table[i] = xlib_rgb24(palette[i*3], palette[i*3+1],
						palette[i*3+2]);
	}

	if (x_visinfo->class == PseudoColor && x_visinfo->depth == 8) {
		if (palette != current_palette) {
			memcpy(current_palette, palette, 768);
		}
		for (i=0 ; i<256 ; i++)	{
			colors[i].pixel	= i;
			colors[i].flags	= DoRed|DoGreen|DoBlue;
			colors[i].red	= vid_gamma[palette[i*3]] * 256;
			colors[i].green	= vid_gamma[palette[i*3+1]] * 256;
			colors[i].blue	= vid_gamma[palette[i*3+2]] * 256;
		}
		XStoreColors(x_disp, x_cmap, colors, 256);
	}
}


/*
  Called at shutdown
*/
void
VID_Shutdown(void)
{
	Sys_Printf("VID_Shutdown\n");
	if (x_disp) {
		XAutoRepeatOn(x_disp);
		x11_close_display();
		x_disp = 0;
	}
}

static int config_notify=0;
static int config_notify_width;
static int config_notify_height;

/*
  Flushes the given rectangles from the view buffer to the screen.
*/
void
VID_Update(vrect_t *rects)
{
	/* If the window changes dimension, skip this frame. */
	if (config_notify) {
		fprintf(stderr, "config notify\n");
		config_notify = 0;
		vid.width = config_notify_width & ~7;
		vid.height = config_notify_height;
		if (doShm)
			ResetSharedFrameBuffers();
		else
			ResetFrameBuffer();
		vid.rowbytes = x_framebuffer[0]->bytes_per_line;
		vid.buffer = x_framebuffer[current_framebuffer]->data;
		vid.conbuffer = vid.buffer;
		vid.conwidth = vid.width;
		vid.conheight = vid.height;
		vid.conrowbytes = vid.rowbytes;
		vid.recalc_refdef = 1;	/* force a surface cache flush */
		Con_CheckResize();
		Con_Clear_f();
		return;
	}

	if (doShm) {
		while (rects) {
			if (x_visinfo->depth == 16) {
				st2_fixup(x_framebuffer[current_framebuffer],
					  rects->x, rects->y, rects->width,
					  rects->height);
			} else if (x_visinfo->depth == 24) {
				st3_fixup(x_framebuffer[current_framebuffer],
					  rects->x, rects->y, rects->width,
					  rects->height);
			}
			if (!XShmPutImage(x_disp, x_win, x_gc,
				x_framebuffer[current_framebuffer],
					  rects->x, rects->y,
					  rects->x, rects->y,
					  rects->width, rects->height, True)) {
				Sys_Error("VID_Update: XShmPutImage failed\n");
			}
			oktodraw = false;
			while (!oktodraw) x11_process_event();
			rects = rects->pnext;
		}
		current_framebuffer = !current_framebuffer;
		vid.buffer = x_framebuffer[current_framebuffer]->data;
		vid.conbuffer = vid.buffer;
		XSync(x_disp, False);
	} else {
		while (rects) {
			if (x_visinfo->depth == 16) {
				st2_fixup(x_framebuffer[current_framebuffer],
					  rects->x, rects->y, rects->width,
					  rects->height);
			} else if (x_visinfo->depth == 24) {
				st3_fixup(x_framebuffer[current_framebuffer],
					  rects->x, rects->y, rects->width,
					  rects->height);
			}
			XPutImage(x_disp, x_win, x_gc, x_framebuffer[0],
				  rects->x, rects->y, rects->x, rects->y,
				  rects->width, rects->height);
			rects = rects->pnext;
		}
		XSync(x_disp, False);
	}
}

static int dither;

void
VID_DitherOn(void)
{
	if (dither == 0) {
		vid.recalc_refdef = 1;
		dither = 1;
	}
}


void
VID_DitherOff(void)
{
	if (dither) {
		vid.recalc_refdef = 1;
		dither = 0;
	}
}

void VID_InitCvars ()
{
	// It may not look like it, but this is important
}

void    
VID_LockBuffer ( void )
{       
}       

void
VID_UnlockBuffer ( void )
{       
}       

