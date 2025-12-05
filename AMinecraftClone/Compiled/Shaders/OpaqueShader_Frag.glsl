#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float shadeValue;

uniform sampler2D textureAtlas;

void main() {

	vec4 color = texture(textureAtlas, TexCoords) * shadeValue;
	if(color.a < 0.1)
		discard;
		
	FragColor = color;
}
