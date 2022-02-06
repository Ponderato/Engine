#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 modelM;
uniform mat4 viewM;
uniform mat4 projM;
uniform mat4 normalM;

out vec3 normal;
out vec3 fragPos;

void main()
{
	normal = mat3(normalM) * aNormal;
	fragPos = (modelM * vec4(aPos, 1.0)).xyz;

	gl_Position =  projM * viewM * modelM * vec4(aPos, 1.0);
}