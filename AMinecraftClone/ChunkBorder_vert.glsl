#version 330 core

const vec3 borderLinesVerts[24] = vec3[](
    // bottom ring
    vec3( 0.0,   0.0,   0.0 ), vec3(16.0,   0.0,   0.0 ), // 0-1
    vec3(16.0,   0.0,   0.0 ), vec3(16.0,   0.0,  16.0 ), // 1-2
    vec3(16.0,   0.0,  16.0 ), vec3( 0.0,   0.0,  16.0 ), // 2-3
    vec3( 0.0,   0.0,  16.0 ), vec3( 0.0,   0.0,   0.0 ), // 3-0

    // top ring
    vec3( 0.0, 128.0,   0.0 ), vec3(16.0, 128.0,   0.0 ), // 4-5
    vec3(16.0, 128.0,   0.0 ), vec3(16.0, 128.0,  16.0 ), // 5-6
    vec3(16.0, 128.0,  16.0 ), vec3( 0.0, 128.0,  16.0 ), // 6-7
    vec3( 0.0, 128.0,  16.0 ), vec3( 0.0, 128.0,   0.0 ), // 7-4

    // verticals
    vec3( 0.0,   0.0,   0.0 ), vec3( 0.0, 128.0,   0.0 ), // 0-4
    vec3(16.0,   0.0,   0.0 ), vec3(16.0, 128.0,   0.0 ), // 1-5
    vec3(16.0,   0.0,  16.0 ), vec3(16.0, 128.0,  16.0 ), // 2-6
    vec3( 0.0,   0.0,  16.0 ), vec3( 0.0, 128.0,  16.0 )  // 3-7
);

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(borderLinesVerts[gl_VertexID], 1.0);
}