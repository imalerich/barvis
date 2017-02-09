#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "util/window.h"
 
int main(int argc, const char ** argv)  {

    init_window();

    // Make sure we show up under the bar.
    XLowerWindow(dpy, win);

    // Main run loop.
    while (true) {
	XEvent xev;
	XWindowAttributes gwa;

	XNextEvent(dpy, &xev);
	if (xev.type == Expose) {
	    XGetWindowAttributes(dpy, win, &gwa);
	    glViewport(0, 0, gwa.width, gwa.height);
	    glClearColor(0.0, 0.0, 0.0, 1.0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    update_screen();
	    glXSwapBuffers(dpy, win);
	}

	sleep(1);
    }
}
