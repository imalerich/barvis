#version 330

in vec2 TexCoord;
out vec4 OutColor;

uniform sampler2D tex;
uniform vec2 size;

#define BORDER rgb(133, 209, 235)

vec4 rgb(float R, float G, float B) {
    return vec4(R/255.0, G/255.0, B/255.0, 1.0);
}

void main() {
    // Display the texture we've rendered to on the screen.
    OutColor = texture(tex, vec2(TexCoord.x, 1.0 - TexCoord.y));

    if (TexCoord.y >= (size.y-4)/(size.y)) {
	OutColor = BORDER;
    }
}
