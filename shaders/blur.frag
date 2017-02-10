#version 330

in vec2 TexCoord;
layout(location=0) out vec4 OutColor;

uniform sampler2D tex;

void main() {
    // Display the texture we've rendered to on the screen.
    OutColor = texture(tex, vec2(TexCoord.x, 1.0 - TexCoord.y)) * 0.5 
	+ 0.5 * vec4(1.0, 0.0, 0.0, 1.0); // Just tint it red for now.
}
