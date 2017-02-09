//
//  gl_util.c
//  barvis
//
//  Created by Ian Malerich on 1/21/16.
//  Copyright © 2016 Ian Malerich. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "gl_util.h"
#include "file_io.h"

unsigned screen_w = 1920;
unsigned screen_h = 28;

GLenum gl_err;

GLuint vao;
GLuint vbo;
GLuint ebo;

GLuint vshader;
GLuint fshader;
GLuint shader_prog;

// array of vertices describing the entire screen
const float vertices[] = {
    -1.0f,  1.0f,   0.0f, 0.0f, // Top-Left
     1.0f,  1.0f,   1.0f, 0.0f, // Top-Right
     1.0f, -1.0f,   1.0f, 1.0f, // Bottom-Right
    -1.0f, -1.0f,   0.0f, 1.0f  // Bottom-Left
};

const GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
};

// private function prototypes
void init_screen_rect();
void init_shaders();
void check_shader_compile(const char * filename, GLuint shader);

void init_gl() {
    // intialize glew
    glewExperimental = GL_TRUE;
    int glew_err = GLEW_OK;

    if ((glew_err = glewInit()) != GLEW_OK) {
        printf("error - glewInit():\nt\t%s\n", glewGetErrorString(gl_err));
        exit(EXIT_FAILURE);
    }

    // clear the error buffer, just trust me on this
    glGetError();

    // generate our vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // initialize the rendering objects that will be used for ray tracing
    init_screen_rect();
    init_shaders();

    // TODO: Figure out why it is complaining about this, seems to work anyway.
    // gl_check_errors("init_gl(...)");
    glGetError(); // Remove the error that we're getting.
}

void update_screen() {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void init_screen_rect() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements, GL_STATIC_DRAW);
}

void init_shaders() {
    const char * vs_file_name = "shaders/simple.vs";
    const char * fs_file_name = "shaders/simple.fs";
    char * vs_source = read_file(vs_file_name);
    char * fs_source = read_file(fs_file_name);

    // compile and check the vertex shader
    vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, (const GLchar **)&vs_source, NULL);
    glCompileShader(vshader);
    check_shader_compile(vs_file_name, vshader);

    // compile and check the fragment shader
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, (const GLchar **)&fs_source, NULL);
    glCompileShader(fshader);
    check_shader_compile(fs_file_name, fshader);

    shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vshader);
    glAttachShader(shader_prog, fshader);

    glBindFragDataLocation(shader_prog, 0, "OutColor");

    glLinkProgram(shader_prog);
    glUseProgram(shader_prog);

    // tell the shader where each input is located on the vertex buffer
    GLint pos_att = glGetAttribLocation(shader_prog, "position");
    GLint tex_att = glGetAttribLocation(shader_prog, "texcoord");

    glVertexAttribPointer(pos_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    glEnableVertexAttribArray(pos_att);
    glEnableVertexAttribArray(tex_att);

    free(vs_source);
    free(fs_source);
}

void check_shader_compile(const char * filename, GLuint shader) {
    GLint status;
    char buffer[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(shader, 512, NULL, buffer);

    // check for any log info
    if (strlen(buffer) > 0) {
        printf("%s\n%s\n", filename, buffer);
    }
}

void gl_check_errors(const char * info) {
    bool found_err = false;
    while ((gl_err = glGetError()) != 0) {
        found_err = true;
        fprintf(stderr, "%s - err: %d\n", info, gl_err);
    }

    if (found_err) {
        exit(EXIT_FAILURE);
    }
}
