#version 330

in vec2 TexCoord;
out vec4 OutColor;

uniform sampler2D tex;

void main() {
    // Display the texture we've rendered to on the screen.
    OutColor = texture(tex, vec2(TexCoord.x, 1.0 - TexCoord.y));
}
