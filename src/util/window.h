#ifndef XLIB_UTIL_H
#define XLIB_UTIL_H

#include <stdbool.h>

#include<X11/X.h>
#include<X11/Xlib.h>

#include "gl_util.h"

extern Display * dpy;
extern Window win;
extern GLXContext glc;

/**
 * Initializes a desktop window, 
 * creates an OpenGL context for that window.
 * Then performs some basic setup on that context.
 * This method will call the init_gl(..) method of
 * gl_util.h, so there is no need to call it after this
 * method completes.
 */
void init_window();

#endif
