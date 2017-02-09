# version 150

in vec2 TexCoord;
out vec4 OutColor;

void main() {
    // output color is exclusively dependent on the input texture
    // for our ray tracer, this will be the ray traced scene

    if (TexCoord.y >= 27.0/28.0) {
	OutColor = vec4(0.8, 0.8, 0.8, 1.0);
    } else {
	OutColor = vec4(TexCoord.x, 0.0, TexCoord.y, 1.0);
    }
}
