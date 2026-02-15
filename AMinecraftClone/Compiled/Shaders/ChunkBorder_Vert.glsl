#version 330 core

const float epsilon = 0.001;

const vec3 borderLinesVerts[24] = vec3[](
    // bottom ring
    vec3( 0.0 - epsilon,   0.0 - epsilon,   0.0 - epsilon ), vec3(1.0 + epsilon,   0.0 - epsilon,   0.0 - epsilon ), // 0-1
    vec3(1.0 + epsilon,   0.0 - epsilon,   0.0 - epsilon ), vec3(1.0 + epsilon,   0.0 - epsilon,  1.0 + epsilon ), // 1-2
    vec3(1.0 + epsilon,   0.0 - epsilon,  1.0 + epsilon ), vec3( 0.0 - epsilon,   0.0 - epsilon,  1.0 + epsilon ), // 2-3
    vec3( 0.0 - epsilon,   0.0 - epsilon,  1.0 + epsilon ), vec3( 0.0 - epsilon,   0.0 - epsilon,   0.0 - epsilon ), // 3-0

    // top ring
    vec3( 0.0 - epsilon, 1.0 + epsilon,   0.0 - epsilon ), vec3(1.0 + epsilon, 1.0 + epsilon,   0.0 - epsilon ), // 4-5
    vec3(1.0 + epsilon, 1.0 + epsilon,   0.0 - epsilon ), vec3(1.0 + epsilon, 1.0 + epsilon,  1.0 + epsilon ), // 5-6
    vec3(1.0 + epsilon, 1.0 + epsilon,  1.0 + epsilon ), vec3( 0.0 - epsilon, 1.0 + epsilon,  1.0 + epsilon ), // 6-7
    vec3( 0.0 - epsilon, 1.0 + epsilon,  1.0 + epsilon ), vec3( 0.0 - epsilon, 1.0 + epsilon,   0.0 - epsilon ), // 7-4

    // verticals
    vec3( 0.0 - epsilon,   0.0 - epsilon,   0.0 - epsilon ), vec3( 0.0 - epsilon, 1.0 + epsilon,   0.0 - epsilon ), // 0-4
    vec3(1.0 + epsilon,   0.0 - epsilon,   0.0 - epsilon ), vec3(1.0 + epsilon, 1.0 + epsilon,   0.0 - epsilon ), // 1-5
    vec3(1.0 + epsilon,   0.0 - epsilon,  1.0 + epsilon ), vec3(1.0 + epsilon, 1.0 + epsilon,  1.0 + epsilon ), // 2-6
    vec3( 0.0 - epsilon,   0.0 - epsilon,  1.0 + epsilon ), vec3( 0.0 - epsilon, 1.0 + epsilon,  1.0 + epsilon )  // 3-7
);

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(borderLinesVerts[gl_VertexID], 1.0);
}