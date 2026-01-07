#version 330 core

out vec2 UV;

vec2 QuadVerts[] = vec2[](
	vec2(-1, -1), vec2(1, -1), vec2(1, 1),
	vec2(1, 1), vec2(-1, 1), vec2(-1, -1)
);

vec2 QuadUVs[] = vec2[](
    vec2(0, 0), vec2(1, 0), vec2(1, 1),
    vec2(1, 1), vec2(0, 1), vec2(0, 0)
);

void main() {
	UV = QuadUVs[gl_VertexID];
	gl_Position = vec4(QuadVerts[gl_VertexID], -1.0, 1.0);
}