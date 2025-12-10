#version 330 core

vec3 diamondVertices[] = vec3[](
    // top half
    vec3( 0.0,  1.0,  0.0 ), vec3( 1.0,  0.0,  0.0 ), vec3( 0.0,  0.0,  1.0 ),
    vec3( 0.0,  1.0,  0.0 ), vec3( 0.0,  0.0,  1.0 ), vec3( -1.0, 0.0,  0.0 ),
    vec3( 0.0,  1.0,  0.0 ), vec3( -1.0, 0.0,  0.0 ), vec3( 0.0,  0.0, -1.0 ),
    vec3( 0.0,  1.0,  0.0 ), vec3( 0.0,  0.0, -1.0 ), vec3( 1.0,  0.0,  0.0 ),

    // bottom half
    vec3( 0.0, -1.0,  0.0 ), vec3( 0.0,  0.0,  1.0 ), vec3( 1.0,  0.0,  0.0 ),
    vec3( 0.0, -1.0,  0.0 ), vec3( -1.0, 0.0,  0.0 ), vec3( 0.0,  0.0,  1.0 ),
    vec3( 0.0, -1.0,  0.0 ), vec3( 0.0,  0.0, -1.0 ), vec3( -1.0, 0.0,  0.0 ),
    vec3( 0.0, -1.0,  0.0 ), vec3( 1.0,  0.0,  0.0 ), vec3( 0.0,  0.0, -1.0 )
);

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * model * vec4(diamondVertices[gl_VertexID], 1.0);
}