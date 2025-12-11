#version 330 core

uniform mat4 model;
uniform mat4 proj; //orthographic projection

out vec2 vUV;

const vec3 cubeVerts[18] = vec3[](
    vec3(0.0, 0.0, 0.5), vec3(0.5, 0.0, 0.5), vec3(0.5, 0.5, 0.5),
    vec3(0.0, 0.0, 0.5), vec3(0.5, 0.5, 0.5), vec3(0.0, 0.5, 0.5),

    
    vec3(0.5, 0.0, 0.5), vec3(0.5, 0.0, 0.0), vec3(0.5, 0.5, 0.0),
    vec3(0.5, 0.0, 0.5), vec3(0.5, 0.5, 0.0), vec3(0.5, 0.5, 0.5),

    
    vec3(0.0, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.0), 
	vec3(0.0, 0.5, 0.5), vec3(0.5, 0.5, 0.0), vec3(0.0, 0.5, 0.0),
);

const vec2 cubeUVs[18] = vec2[](
    vec2(0,0), vec2(1.0/16.0,0), vec2(1.0/16.0, 1.0/16.0),
    vec2(0,0), vec2(1.0/16.0,1.0/16.0), vec2(0,1.0/16.0),

    vec2(0,0), vec2(1.0/16.0,0), vec2(1.0/16.0,1.0/16.0),
    vec2(0,0), vec2(1.0/16.0,1.0/16.0), vec2(0,1.0/16.0),

    vec2(0,0), vec2(1.0/16.0,0), vec2(1.0/16.0,1.0/16.0),
    vec2(0,0), vec2(1.0/16.0,1.0/16.0), vec2(0,1.0/16.0),
);

void main() {
    gl_Position = vec4(cubeVerts[gl_VertexID], 1.0);
    vUV = cubeUVs[gl_VertexID];
}