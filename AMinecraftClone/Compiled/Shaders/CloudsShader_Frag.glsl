#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D cloudsTexture;
uniform float Time;
uniform vec3 playerWorldPos;

void main() {
	vec2 ScrollOffset = vec2(playerWorldPos.x, playerWorldPos.z) * 0.001 + vec2(Time * 0.005, 0.0);
	vec2 uv = UV + ScrollOffset;
	vec4 color = texture(cloudsTexture, uv);
	if(color.a < 0.1) 
		discard;
	FragColor = color;
}