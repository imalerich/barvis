#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "gl_util.h"

/**
 * Encapsulates the frame buffer to render to
 * as well as the texture associated with that frame buffer.
 */
struct frame_buffer {
    GLuint buffer;
    GLuint tex;
    unsigned width;
    unsigned height;
};

/**
 * Constructs a new frame buffer and corresponding texture
 * for the given texture size.
 */
struct frame_buffer create_frame_buffer(const unsigned width, const unsigned height);

/**
 * Binds the input frame buffer, and sets the buffers
 * corresponding texture as the render target.
 */
void bind_frame_buffer(struct frame_buffer buff);

/**
 * Reset the current frame buffer and 
 * render to the screen.
 * This call will clear the screen buffer.
 */
void bind_screen_buffer(unsigned screenw, unsigned screenh);

#endif
