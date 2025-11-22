#version 330 core

out vec4 FragColor;

in vec3 fragPos;

uniform float time;

vec3 getSkyColor(float t)
{
    float sunAngle = t * 2.0 * 3.1415926;

    
    float f = cos(sunAngle) * 0.5 + 0.5;


    vec3 topColor = vec3(0.6, 0.8, 1.0);
    vec3 bottomColor = vec3(0.15, 0.2, 0.3);


    float h = fragPos.y * 0.5 + 0.5;
    return mix(bottomColor, topColor, h) * f;
}

void main() {
	vec3 color = getSkyColor(time);
	
	FragColor = vec4(color, 1.0);
}