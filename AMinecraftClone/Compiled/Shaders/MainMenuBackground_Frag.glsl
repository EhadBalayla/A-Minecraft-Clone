#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D dirtTex;

void main() {
	FragColor = texture(dirtTex, UV * vec2(14.0, 7.875)) * 0.25;
}