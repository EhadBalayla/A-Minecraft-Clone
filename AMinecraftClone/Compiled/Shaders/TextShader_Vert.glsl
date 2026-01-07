#version 330 core

out vec2 transUV;

uniform vec2 UV0;
uniform vec2 UV1;

uniform mat4 projection;
uniform mat4 model;

vec2 QuadVerts[] = vec2[](
	vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0.5, 0.5),
	vec2(0.5, 0.5), vec2(-0.5, 0.5), vec2(-0.5, -0.5)
);

void main() {
	if (gl_VertexID == 0 || gl_VertexID == 5) transUV = UV0;
    else if (gl_VertexID == 1) transUV = vec2(UV1.x, UV0.y);
    else if (gl_VertexID == 2 || gl_VertexID == 3) transUV = UV1;
    else transUV = vec2(UV0.x, UV1.y);
	gl_Position = projection * model * vec4(QuadVerts[gl_VertexID], -1.0, 1.0);
}