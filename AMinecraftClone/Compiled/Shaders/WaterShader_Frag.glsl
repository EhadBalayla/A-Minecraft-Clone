#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureAtlas;
uniform float Time;

vec2 PanUVClamped(vec2 uv, vec2 tileOrigin, vec2 direction, float speed, float time);

void main() {
    vec2 tileOrigin = vec2(14.0, 13.0) / 16.0;
    vec2 uv = TexCoords;  
	vec2 actualUV = PanUVClamped(uv, tileOrigin, vec2(1.0, 0.0), 0.05, Time);
	FragColor = vec4(texture(textureAtlas, actualUV).rgb * 0.75, 0.8);
}

vec2 PanUVClamped(vec2 uv, vec2 tileOrigin, vec2 direction, float speed, float time) {
    float tileSize = 1.0 / 16.0;
    vec2 offset = direction * speed * time;
    return tileOrigin + mod(uv - tileOrigin + offset, tileSize);
}