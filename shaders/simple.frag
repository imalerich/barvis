#version 330

in vec2 TexCoord;
out vec4 OutColor;

uniform sampler2D tex;

void main() {
    if (TexCoord.y >= 27/28.0) {
	OutColor = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
	// Display the texture we've rendered to on the screen.
	OutColor = texture(tex, vec2(TexCoord.x, 1.0 - TexCoord.y));
    }
}
