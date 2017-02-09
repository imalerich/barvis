# version 150

in vec2 TexCoord;
out vec4 OutColor;

void main() {
    // output color is exclusively dependent on the input texture
    // for our ray tracer, this will be the ray traced scene
    OutColor = vec4(TexCoord.x, 0.0, TexCoord.y, 1.0);
}
