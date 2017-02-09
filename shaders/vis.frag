#version 330

in vec2 TexCoord;
layout(location=0) out vec4 OutColor;

#define M_PI 3.1415926535897932384626433832795
#define M_E 2.71828

#define BANDS 80
#define THRESHOLD -80.0

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
    float K = 0.015;

    float sum = 0.0;
    for (int i=0; i<BANDS; i++) {
	float xi = (1.0 + 2.0*i) * 0.5 * h;
	float P = (pulse[i] - THRESHOLD) / 60.0;
	sum += K * P * gaussian(xi, sigma, TexCoord.x);
    }

    float y = 1.0 - sum;

    if (TexCoord.y >= 27.0/28.0) {
	OutColor = rgb(7, 104, 97);
    } else if (TexCoord.y >= y) {
	OutColor = 
	    y * rgb(7, 104, 97) +
	    sum * rgb(254, 10, 49);
    } else {
	float c = TexCoord.y - y;
	OutColor = 
	    y * rgb(0, 0, 0) +
	    sum * rgb(10, 10, 10);
    }
}
