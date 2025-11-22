#version 330 core

out vec4 FragColor;

uniform float time;

void main() {
	float alpha = (sin(time) + 1) / 2;
	
	vec3 colorA = vec3(0.0, 0.25, 1.0);
	vec3 colorB = vec3(1.0, 0.6, 0.1);
	
	FragColor = vec4(mix(colorA, colorB, alpha), 1.0);
}