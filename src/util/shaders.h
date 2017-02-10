#ifndef SHADERS_H
#define SHADERS_H

#include "gl_util.h"

/**
 * Compiles a new shader program using the input vertex and fragment
 * shader files.
 * \param vert_name File name and path for the vertex shader.
 * \param frag_name File name and path for the fragment shader.
 * \return Identifier for the compiled shader program.
 */
GLuint compile_shader(const char * vert_name, const char * frag_name);

/**
 * Checks whether or not the input shader was successfuly compiled.
 * If not an error message will be printed to the stderr stream
 * with the compile error.
 * Note that this method will NOT terminate the application.
 */
void check_shader_compile(const char * filename, GLuint shader);

#endif
