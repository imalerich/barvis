#version 330

in vec2 TexCoord;
layout(location=0) out vec4 OutColor;

#define BANDS 200
#define THRESHOLD -80.0
#define SCALE 50.0

#define M_PI 3.1415926535897932384626433832795
#define M_E 2.71828

#define NUM_COLORS 5
#define C1 rgb(252, 58, 87)
#define C2 rgb(249, 19, 55)
#define C3 rgb(135, 1, 36)
#define C4 rgb(107, 8, 46)
#define C5 rgb(77, 1, 30)

#define BG rgb(8, 1, 20);

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
    vec4 C[NUM_COLORS] = vec4[](C5, C4, C3, C2, C1);

    if (TexCoord.y >= y) {
	float h = (1.0 - y) / NUM_COLORS;
	float yprime = TexCoord.y - y - (h/2.0);
	int i = int((yprime / sum) * NUM_COLORS);
	float p = 1.0 - (yprime - (float(i) * h)) / h;

	if (i == NUM_COLORS-1) {
	    OutColor = C[i];
	} else {
	    OutColor = p * C[i] + (1.0 - p) * C[i+1];
	}

    } else {
	OutColor = BG;
    }
}
