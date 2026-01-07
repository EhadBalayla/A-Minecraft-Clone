#version 330 core

out vec4 FragColor;

uniform sampler2D FontTexture;

uniform vec4 Tint;

in vec2 transUV;

void main() {
	vec4 color = texture(FontTexture, transUV);
	if(color.a < 0.1) discard;
	FragColor = color * Tint;
}