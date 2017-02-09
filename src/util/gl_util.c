#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "window.h"
#include "gl_util.h"
#include "file_io.h"

#define VIS_SCALING 1.0

unsigned screen_w = 1920;
unsigned screen_h = 28;
GLuint vis_tex;

GLenum gl_err;

GLuint vao;
GLuint vbo;
GLuint ebo;

GLuint frame_buff;

GLuint vshader;
GLuint vis_prog;
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
void init_vis_tex();

void init_vshader();
void init_vis_shader();
void init_simple_shader();

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
    init_vshader();
    init_vis_shader();
    init_simple_shader();
    init_vis_tex();
    gl_check_errors("init_gl(...)");
}

void update_screen() {
    // Get window properties from XLib.
    XWindowAttributes gwa;
    XGetWindowAttributes(dpy, win, &gwa);

    // Clear the screen.
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Render the visualizer in 'vis_tex'.
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buff);
    glViewport(0, 0, VIS_SCALING * gwa.width, VIS_SCALING * gwa.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vis_tex);

    glUseProgram(vis_prog);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Now draw 'vis_tex' on the back buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, gwa.width, gwa.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(simple_prog);
    glBindTexture(GL_TEXTURE_2D, vis_tex);
    glUniform1i(glGetUniformLocation(simple_prog, "tex"), 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Rendering is done, swap buffers.
    glXSwapBuffers(dpy, win);

    gl_check_errors("update_screen(...)");
}

void init_vis_tex() {
    glGenFramebuffers(1, &frame_buff);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buff);

    glGenTextures(1, &vis_tex);
    glBindTexture(GL_TEXTURE_2D, vis_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, VIS_SCALING * screen_w, VIS_SCALING * screen_h, 0, GL_RGB, GL_FLOAT, NULL);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, vis_tex, 0);
    GLenum draw_buff[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buff);
}

void init_screen_rect() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements, GL_STATIC_DRAW);
}

void init_vshader() {
    const char * vs_file_name = "shaders/simple.vert";
    char * vs_source = read_file(vs_file_name);

    // compile and check the vertex shader
    vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, (const GLchar **)&vs_source, NULL);
    glCompileShader(vshader);
    check_shader_compile(vs_file_name, vshader);

    free(vs_source);
}

void init_simple_shader() {
    GLuint fshader;
    const char * fs_file_name = "shaders/simple.frag";
    char * fs_source = read_file(fs_file_name);

    // compile and check the fragment shader
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, (const GLchar **)&fs_source, NULL);
    glCompileShader(fshader);
    check_shader_compile(fs_file_name, fshader);

    simple_prog = glCreateProgram();
    glAttachShader(simple_prog, vshader);
    glAttachShader(simple_prog, fshader);

    glBindFragDataLocation(simple_prog, 0, "OutColor");

    glLinkProgram(simple_prog);
    glUseProgram(simple_prog);

    // tell the shader where each input is located on the vertex buffer
    GLint pos_att = glGetAttribLocation(vis_prog, "position");
    GLint tex_att = glGetAttribLocation(vis_prog, "texcoord");

    glVertexAttribPointer(pos_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    glEnableVertexAttribArray(pos_att);
    glEnableVertexAttribArray(tex_att);

    free(fs_source);
}

void init_vis_shader() {
    GLuint fshader;
    const char * fs_file_name = "shaders/vis.frag";
    char * fs_source = read_file(fs_file_name);

    // compile and check the fragment shader
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, (const GLchar **)&fs_source, NULL);
    glCompileShader(fshader);
    check_shader_compile(fs_file_name, fshader);

    vis_prog = glCreateProgram();
    glAttachShader(vis_prog, vshader);
    glAttachShader(vis_prog, fshader);

    glBindFragDataLocation(vis_prog, 0, "OutColor");

    glLinkProgram(vis_prog);
    glUseProgram(vis_prog);

    // tell the shader where each input is located on the vertex buffer
    GLint pos_att = glGetAttribLocation(vis_prog, "position");
    GLint tex_att = glGetAttribLocation(vis_prog, "texcoord");

    glVertexAttribPointer(pos_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    glEnableVertexAttribArray(pos_att);
    glEnableVertexAttribArray(tex_att);

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
