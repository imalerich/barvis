#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include<X11/X.h>
#include<X11/Xlib.h>

#define GLEW_STATIC
#include<GL/glew.h>

#include "gl_util.h"

Display			* dpy;
Window			root;
GLint			att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo		* vi;
Colormap		cmap;
XSetWindowAttributes	swa;
Window			win;
GLXContext		glc;
XWindowAttributes	gwa;
XEvent			xev;
 
int main(int argc, const char ** argv)  {
    dpy = XOpenDisplay(NULL);
    root = DefaultRootWindow(dpy);
    vi = glXChooseVisual(dpy, 0, att);
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;
    swa.override_redirect = true;

    // Create the window.
    win = XCreateWindow(dpy, root, 1366, 0, 1920, 28, 0, 
	vi->depth, InputOutput, vi->visual, 
	CWColormap | CWEventMask | CWOverrideRedirect, 
	&swa);

    XClassHint * hint = XAllocClassHint();
    hint->res_name = "barvis";
    hint->res_class = "dektop";
    XSetClassHint(dpy, win, hint);

    XMapWindow(dpy, win);
    XStoreName(dpy, win, "barvis");

    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

    init_gl();

    // Make sure we show up under the bar.
    XLowerWindow(dpy, win);

    // Main run loop.
    while (1) {
	XNextEvent(dpy, &xev);

	if (xev.type == Expose) {
	    XGetWindowAttributes(dpy, win, &gwa);
	    glViewport(0, 0, gwa.width, gwa.height);
	    glClearColor(0.0, 0.0, 0.0, 1.0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    update_screen();
	    glXSwapBuffers(dpy, win);
	}
    }
}
