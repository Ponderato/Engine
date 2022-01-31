#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//out vec3 color;
out vec2 texCoord;

uniform mat4 modelM;
uniform mat4 viewM;
uniform mat4 projM;

void main()
{
	gl_Position =  projM * viewM * modelM * vec4(aPos, 1.0);
	//color = aColor;
	texCoord = aTexCoord;
}