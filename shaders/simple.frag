#version 450

in vec3 color;
out vec4 fragColor; // out: pixel color

void main()
{
	// 
	fragColor = vec4(color, 1.0);	
}