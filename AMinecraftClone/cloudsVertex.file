#version 330 core

vec3 verticies[] = vec3[](
    vec3(-1.0, 0.0, -1.0),
    vec3(1.0, 0.0, -1.0),
    vec3(1.0, 0.0,  1.0),

    vec3(1.0, 0.0,  1.0),
    vec3(-1.0, 0.0,  1.0),
    vec3(-1.0, 0.0, -1.0)
);

vec2 uvs[] = vec2[](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0),

    vec2(1.0, 1.0),
    vec2(0.0, 1.0),
    vec2(0.0, 0.0)
);

out vec2 UV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	UV = uvs[gl_VertexID];
	gl_Position = projection * view * model * vec4(verticies[gl_VertexID], 1.0);
}