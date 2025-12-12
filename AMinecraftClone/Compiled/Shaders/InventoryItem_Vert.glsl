#version 330 core

uniform mat4 model;
uniform mat4 proj;

uniform int TopIDX;
uniform int RightIDX;
uniform int LeftIDX;

out vec2 vUV;
out vec2 uvOffset;
out float shading;


vec2 positions[18] = vec2[](
    //top face
    vec2(0.0, 0.5),
    vec2(0.446, 0.273),
    vec2(0.0,  0.05),

    vec2(0.0, 0.05),
    vec2(-0.446,  0.273),
    vec2(0.0, 0.5),

    //right face
    vec2(0.0,  0.05),
    vec2(0.446, 0.273),
    vec2(0.446, -0.273),

    vec2(0.446, -0.273),
    vec2(0.0, -0.5),
    vec2(0.0,  0.05),

    //left face
    vec2(0.0,  0.05),
    vec2(0.0, -0.5),
    vec2(-0.446, -0.273),

    vec2(-0.446, -0.273),
    vec2(-0.446, 0.273),
    vec2(0.0,  0.05)
);

vec2 uvs[18] = vec2[](
    //top face
    vec2(0.0, 0.0),
    vec2(0.0625, 0.0),
    vec2(0.0625, 0.0625),

    vec2(0.0625, 0.0625),
    vec2(0.0, 0.0625),
    vec2(0.0, 0.0),

    //right face
	vec2(0.0625, 0.0),
	vec2(0.0, 0.0),
	vec2(0.0, 0.0625),
	
    vec2(0.0, 0.0625),
    vec2(0.0625, 0.0625),
    vec2(0.0625, 0.0),

    //left face
    vec2(0.0, 0.0),
    vec2(0.0, 0.0625),
    vec2(0.0625, 0.0625),

	vec2(0.0625, 0.0625),
    vec2(0.0625, 0.0),
    vec2(0.0, 0.0)
);

void main() {
	int faceIDX;

	if(gl_VertexID < 6) { faceIDX = TopIDX;  shading = 1.0; }
	else if(gl_VertexID < 12) { faceIDX = RightIDX;  shading = 0.8; }
	else { faceIDX = LeftIDX;  shading = 0.6; }
	
	float offsetX = float(faceIDX % 16) / 16.0;
	float offsetY = float(faceIDX / 16) / 16.0;
	uvOffset = vec2(offsetX, offsetY);

    gl_Position = proj * model * vec4(positions[gl_VertexID], -1.0, 1.0);
    vUV = uvs[gl_VertexID];
}