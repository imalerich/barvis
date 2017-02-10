#version 330

in vec2 TexCoord;
out vec4 OutColor;

uniform sampler2D tex;
uniform vec2 size;

void main() {
    // Display the texture we've rendered to on the screen.
    OutColor = texture(tex, vec2(TexCoord.x, 1.0 - TexCoord.y));

    if (TexCoord.y >= (size.y-1)/(size.y)) {
	OutColor = 0.3 * vec4(1.0, 1.0, 1.0, 1.0) 
		 + 0.7 * OutColor;
    }
}
