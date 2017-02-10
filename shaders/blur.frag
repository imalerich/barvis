#version 330

in vec2 TexCoord;
layout(location=0) out vec4 OutColor;

uniform sampler2D tex;
uniform vec2 dir;
uniform vec2 size;

#define M_PI 3.1415926535897932384626433832795
#define M_E 2.71828

// Eventually, this should be a static kernel.
// As all coefficients are constant, this should
// greatly improve performance.
float gaussian(float mean, float sigma, float x) {
    float sigma2 = pow(sigma, 2);
    float den = sqrt(2.0 * sigma2 * M_PI);
    float num = pow(M_E, -(pow(x - mean, 2.0)) / (2.0 * sigma2));
    return num / den;
}

#define H_KERNEL_WIDTH 44
#define SCALE 3.0
#define SIGMA 30
#define MEAN 0.0

void main() {
    OutColor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i=-H_KERNEL_WIDTH; i<H_KERNEL_WIDTH; ++i) {
        vec2 texcoord = vec2(TexCoord.x, 1.0 - TexCoord.y) +
            (vec2(1.0/size.x, 1.0/size.y) * dir * (i/SCALE));
        OutColor += texture(tex, texcoord) * gaussian(MEAN, SIGMA, i);
    }

    OutColor.a = 1.0;
}
