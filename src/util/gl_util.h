#ifndef GL_UTIL_H
#define GL_UTIL_H

#define GLEW_STATIC
#include <GL/glew.h>

#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

extern GLuint vis_prog;

/**
 * Initializes an OpenGL context using screen 
 * dimmensions of screen_w and screen_h.
 * \param title Title to use for the window.
 * \param v_sync The swap interval, use 0 to disable vertical sync.
 */
void init_gl();

/**
 * Draws a rectangle over the entire screen using the
 * texture to be filled by OpenCL.
 */
void update_screen();

/**
 * Checks if an operation has produced an error since last
 * checking for an error. If so, this method will print out
 * the information detailed in 'info' as well as the error code
 * to stderr, and then exit with a failure.
 */
void gl_check_errors(const char * info);

#endif
