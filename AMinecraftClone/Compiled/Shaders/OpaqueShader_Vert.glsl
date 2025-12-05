#version 330 core
layout (location = 0) in uint vert;

out vec2 TexCoords;
out float shadeValue;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uvec3 decodePos(uint pos);
uvec3 decodeOffset(uint corner);
vec2 decodeUVoffset(uint texCorn);

void main() {
	uint pos = vert & 0xFFFFu;
	uint texIndex = (vert >> 16) & 0xFFu;
	uint shading = (vert >> 24) & 3u;
	uint texCorner = (vert >> 26) & 3u;
	uint blockCorner = (vert >> 28);
	
	uvec3 actualPos = decodePos(pos);
	uvec3 posOffset = decodeOffset(blockCorner);

	gl_Position = projection * view * model * vec4(actualPos + posOffset, 1.0);
	
	
	if(shading == 0u) shadeValue = 1.0;
	else if(shading == 1u) shadeValue = 0.8;
	else shadeValue = 0.6;
	
	
	uint indexX = texIndex % uint(16);
	uint indexY = texIndex / uint(16);
	float uvX =  float(indexX) / 16;
	float uvY = float(indexY) / 16;
	vec2 uvOffset = decodeUVoffset(texCorner);
	
	TexCoords = vec2(uvX, uvY) + uvOffset;
}


uvec3 decodePos(uint pos) {
	uvec3 ret;
	ret.x = pos & 0xFu;
	ret.z = (pos >> 4) & 0xFu;
	ret.y = (pos >> 8) & 0xFFu;
	
	return ret;
}
uvec3 decodeOffset(uint corner) {
	const uvec3 offsets[8] = uvec3[8](
        uvec3(0,0,0), uvec3(1,0,0), uvec3(1,0,1), uvec3(0,0,1),
        uvec3(0,1,0), uvec3(1,1,0), uvec3(1,1,1), uvec3(0,1,1)
    );
	return offsets[corner];
}
vec2 decodeUVoffset(uint texCorn) {
	const vec2 offsets[4] = vec2[4](
		vec2(0, 0), vec2(0.0625, 0), vec2(0.0625, 0.0625), vec2(0, 0.0625)
	);
	return offsets[texCorn];
}