#version 330 core

layout (location = 0) in vec2 aPos; //position of widgets in screenspace

out vec2 transUV;

uniform vec2 UV0;
uniform vec2 UV1;
uniform vec2 UV2;
uniform vec2 UV3;

uniform mat4 projection; //this is where you get the orthographic projection
uniform mat4 model;

void main() {
	if (gl_VertexID == 0) transUV = UV0;
    else if (gl_VertexID == 1) transUV = UV1;
    else if (gl_VertexID == 2) transUV = UV2;
    else transUV = UV3;
	gl_Position = projection * model * vec4(aPos, -1.0, 1.0);
}