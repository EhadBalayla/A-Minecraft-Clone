#version 330 core

out vec4 FragColor;

uniform sampler2D UITexture;

in vec2 transUV;

void main() {
	FragColor = texture(UITexture, transUV);
}