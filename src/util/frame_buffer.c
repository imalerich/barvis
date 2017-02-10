#include "frame_buffer.h"

struct frame_buffer create_frame_buffer(const unsigned width, const unsigned height) {
    GLuint tex, buff;

    glGenFramebuffers(1, &buff);
    glBindFramebuffer(GL_FRAMEBUFFER, buff);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, NULL);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
    GLenum draw_buff[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buff);

    return (struct frame_buffer){buff, tex, width, height};
}

void bind_frame_buffer(struct frame_buffer buff) {
    glBindFramebuffer(GL_FRAMEBUFFER, buff.buffer);
    glViewport(0, 0, buff.width, buff.height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buff.tex);
}

void bind_screen_buffer(unsigned screenw, unsigned screenh) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screenw, screenh);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
