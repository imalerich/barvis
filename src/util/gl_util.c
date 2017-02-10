#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "window.h"
#include "gl_util.h"
#include "shaders.h"
#include "frame_buffer.h"

struct frame_buffer vis_frame;
struct frame_buffer blur_h_frame;
struct frame_buffer blur_v_frame;

GLuint vao;
GLuint vbo;
GLuint ebo;

GLuint vis_prog;
GLuint blur_prog;
GLuint simple_prog;

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
void check_shader_compile(const char * filename, GLuint shader);
void init_frame_buffers();

void init_vis_shader();
void init_blur_shader();
void init_simple_shader();

//////////////////////////
//	Update		//
//////////////////////////

void update_screen() {
    // Get the window size.
    XWindowAttributes gwa;
    XGetWindowAttributes(dpy, win, &gwa);
    float SIZE[] = { gwa.width, gwa.height };
    glClearColor(0.0, 0.0, 0.0, 1.0);

    //////////////////////////////
    //	Render The Visualizer	//
    //////////////////////////////

    bind_frame_buffer(vis_frame);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(vis_prog);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //////////////////////////////////////
    //	Gaussian Blur Horizontal Pass	//
    //////////////////////////////////////
    
    float RIGHT[] = { 1.0, 0.0 };

    bind_frame_buffer(blur_h_frame);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(blur_prog);
    glBindTexture(GL_TEXTURE_2D, vis_frame.tex);

    glUniform1i(glGetUniformLocation(blur_prog, "tex"), 0);
    glUniform2fv(glGetUniformLocation(blur_prog, "dir"), 1, RIGHT);
    glUniform2fv(glGetUniformLocation(blur_prog, "size"), 1, SIZE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //////////////////////////////////////
    //	Gaussian Blur Vertical Pass	//
    //////////////////////////////////////

    float UP[] = { 0.0, 1.0 };

    bind_frame_buffer(blur_v_frame);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(blur_prog);
    glBindTexture(GL_TEXTURE_2D, blur_h_frame.tex);

    glUniform1i(glGetUniformLocation(blur_prog, "tex"), 0);
    glUniform2fv(glGetUniformLocation(blur_prog, "dir"), 1, UP);
    glUniform2fv(glGetUniformLocation(blur_prog, "size"), 1, SIZE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //////////////////////////////////////
    //	Render to the Back Buffer	//
    //////////////////////////////////////

    bind_screen_buffer(gwa.width, gwa.height);
    glUseProgram(simple_prog);
    glBindTexture(GL_TEXTURE_2D, blur_v_frame.tex);
    glUniform1i(glGetUniformLocation(simple_prog, "tex"), 0);
    glUniform2fv(glGetUniformLocation(simple_prog, "size"), 1, SIZE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Rendering is done, swap buffers.
    glXSwapBuffers(dpy, win);
    gl_check_errors("update_screen(...)");
}

//////////////////////////
//	Init		//
//////////////////////////

void init_gl() {
    // intialize glew
    glewExperimental = GL_TRUE;
    int glew_err = GLEW_OK;

    if ((glew_err = glewInit()) != GLEW_OK) {
        printf("error - glewInit():\nt\t%s\n", glewGetErrorString(glew_err));
        exit(EXIT_FAILURE);
    }

    // clear the error buffer, just trust me on this
    glGetError();

    // generate our vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // initialize the rendering objects that will be used for ray tracing
    init_screen_rect();
    init_vis_shader();
    init_blur_shader();
    init_simple_shader();
    init_frame_buffers();
    gl_check_errors("init_gl(...)");
}

void init_frame_buffers() {
    // Get the window size.
    XWindowAttributes gwa;
    XGetWindowAttributes(dpy, win, &gwa);

    vis_frame = create_frame_buffer(gwa.width, gwa.height);
    blur_h_frame = create_frame_buffer(gwa.width, gwa.height);
    blur_v_frame = create_frame_buffer(gwa.width, gwa.height);
}

void init_screen_rect() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements, GL_STATIC_DRAW);
}

//////////////////////////
//	Shaders		//
//////////////////////////

void init_vis_shader() {
    vis_prog = compile_shader("shaders/simple.vert", "shaders/vis.frag");
    glBindFragDataLocation(vis_prog, 0, "OutColor");

    // tell the shader where each input is located on the vertex buffer
    GLint pos_att = glGetAttribLocation(vis_prog, "position");
    GLint tex_att = glGetAttribLocation(vis_prog, "texcoord");

    glVertexAttribPointer(pos_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    glEnableVertexAttribArray(pos_att);
    glEnableVertexAttribArray(tex_att);
}

void init_blur_shader() {
    blur_prog = compile_shader("shaders/simple.vert", "shaders/blur.frag");
    glBindFragDataLocation(blur_prog, 0, "OutColor");

    // tell the shader where each input is located on the vertex buffer
    GLint pos_att = glGetAttribLocation(blur_prog, "position");
    GLint tex_att = glGetAttribLocation(blur_prog, "texcoord");

    glVertexAttribPointer(pos_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    glEnableVertexAttribArray(pos_att);
    glEnableVertexAttribArray(tex_att);
}

void init_simple_shader() {
    simple_prog = compile_shader("shaders/simple.vert", "shaders/simple.frag");
    glBindFragDataLocation(simple_prog, 0, "OutColor");

    // tell the shader where each input is located on the vertex buffer
    GLint pos_att = glGetAttribLocation(simple_prog, "position");
    GLint tex_att = glGetAttribLocation(simple_prog, "texcoord");

    glVertexAttribPointer(pos_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    glEnableVertexAttribArray(pos_att);
    glEnableVertexAttribArray(tex_att);
}

//////////////////////////
//	Errors		//
//////////////////////////

void gl_check_errors(const char * info) {
    GLenum gl_err;
    bool found_err = false;

    while ((gl_err = glGetError()) != 0) {
        found_err = true;
        fprintf(stderr, "%s - err: %d\n", info, gl_err);
    }

    if (found_err) {
        exit(EXIT_FAILURE);
    }
}
