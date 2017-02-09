#include "window.h"

Display * dpy;
Window win;
GLXContext glc;

void init_window() {
    Window root;
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo * vi;
    Colormap cmap;
    XSetWindowAttributes swa;

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

    init_gl();
}
