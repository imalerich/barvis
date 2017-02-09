#version 330

in vec2 position;
in vec2 texcoord;

out vec2 TexCoord;

void main() {
    // passes the input coordinate on to the vertex shader
    TexCoord = texcoord;
    gl_Position = vec4(position, 0.0, 1.0);
}
