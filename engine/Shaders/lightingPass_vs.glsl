#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

void main()
{
	texCoords = aTexCoords;// * vec2(1.0, -1.0);
	gl_Position = vec4(aPos, 1.0f);
}