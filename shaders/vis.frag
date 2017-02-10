#version 330

in vec2 TexCoord;
layout(location=0) out vec4 OutColor;

// TODO: These should be input uniforms.
#define BANDS 160
#define THRESHOLD -80.0
#define SCALE 60.0

#define M_PI 3.1415926535897932384626433832795
#define M_E 2.71828

// TODO: These should be input uniforms.
#define C1 rgb(252, 58, 87)
#define C2 rgb(52, 88, 229)
#define BG rgb(23, 35, 84);

uniform float pulse[BANDS];

vec4 rgb(float R, float G, float B) {
    return vec4(R/255.0, G/255.0, B/255.0, 1.0);
}

float gaussian(float mean, float sigma, float x) {
    float sigma2 = pow(sigma, 2);
    float den = sqrt(2.0 * sigma2 * M_PI);
    float num = pow(M_E, -(pow(x - mean, 2.0)) / (2.0 * sigma2));
    return num / den;
}

void main() {
    float h = 1.0 / float(BANDS);
    float sigma = h;

    float sum = 0.0;
    float MAX = 0.0;
    for (int i=0; i<BANDS; i++) {
	float xi = (1.0 + 2.0*i) * 0.5 * h;
	float P = (pulse[i] - THRESHOLD) / SCALE;
	float g = gaussian(xi, sigma, TexCoord.x);
	MAX += g;
	sum += P * g;
    }
    sum /= MAX;

    float y = 1.0 - sum;

    if (TexCoord.y >= y) {
	// float h = (1.0 - y) / NUM_COLORS;
	// float yprime = TexCoord.y - y - (h/2.0);
	// int i = int((yprime / sum) * NUM_COLORS);
	// float p = 1.0 - (yprime - (float(i) * h)) / h;

	OutColor = 0.9 * sum * C1 + 1.1 * y * C2;

    } else {
	OutColor = BG;
    }
}
