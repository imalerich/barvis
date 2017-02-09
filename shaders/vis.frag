#version 330

in vec2 TexCoord;
layout(location=0) out vec4 OutColor;

#define BANDS 160
#define THRESHOLD -80

uniform float pulse[BANDS];

vec4 rgb(float R, float G, float B) {
    return vec4(R/255.0, G/255.0, B/255.0, 1.0);
}

void main() {
    int i = max(min(int(TexCoord.x * BANDS), BANDS-1), 0);
    float perc = (pulse[i] - THRESHOLD) / 60.0;
    float y = 1.0 - perc;

    if (TexCoord.y >= 27.0/28.0) {
	OutColor = rgb(7, 104, 97);
    } else if (TexCoord.y >= y) {
	OutColor = 
	    y * rgb(7, 104, 97) +
	    perc * rgb(254, 10, 49);
    } else {
	float c = TexCoord.y - y;
	OutColor = 
	    y * rgb(0, 0, 0) +
	    perc * rgb(10, 10, 10);
    }
}
