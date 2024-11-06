#version 450

layout(location = 0) in vec4 vertexPosition; // 0: attribute nbr
layout(location = 1) in vec3 vertexColor; // 1: attribute nbr

out vec3 color;

void main()
{
	color = vertexColor;
	// gl_Position : NDC coordinate : gl_Position must be set (vec4)
	gl_Position = vertexPosition;
}