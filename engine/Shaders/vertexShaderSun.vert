#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelM;
uniform mat4 viewM;
uniform mat4 projM;

void main()
{
	gl_Position =  projM * viewM * modelM * vec4(aPos, 1.0);
}