#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shaders.h"
#include "file_io.h"

GLuint compile_shader(const char * vert_name, const char * frag_name) {
    char * vs_source = read_file(vert_name);
    char * fs_source = read_file(frag_name);

    // compile and check the vertex shader
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, (const GLchar **)&vs_source, NULL);
    glCompileShader(vshader);
    check_shader_compile(vert_name, vshader);

    // compile and check the fragment shader
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, (const GLchar **)&fs_source, NULL);
    glCompileShader(fshader);
    check_shader_compile(frag_name, fshader);

    free(vs_source);
    free(fs_source);

    // finally create and compile the shader program
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vshader);
    glAttachShader(prog, fshader);

    glLinkProgram(prog);
    glUseProgram(prog);

    return prog;
}

void check_shader_compile(const char * filename, GLuint shader) {
    GLint status;
    char buffer[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(shader, 512, NULL, buffer);

    // check for any log info
    if (strlen(buffer) > 0) {
        fprintf(stderr, "%s\n%s\n", filename, buffer);
    }
}
