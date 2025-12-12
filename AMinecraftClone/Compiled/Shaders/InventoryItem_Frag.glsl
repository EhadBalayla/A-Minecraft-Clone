#version 330 core

out vec4 FragColor;

in vec2 vUV;
in vec2 uvOffset;
in float shading;

uniform sampler2D atlas;

void main() {
	vec2 finalUVs = vUV + uvOffset;
	
	vec4 tex = texture(atlas, finalUVs);
	vec4 color = vec4(tex.rgb * shading, tex.a);
	if(color.a < 0.1) discard;

	FragColor = color;
}