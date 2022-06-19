#version 330 core

//Control to which color buffer the fragment shader writes to.
layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec4 gColorSpec;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform sampler2D matDiffuse1;
uniform sampler2D matSpecular1;

uniform int hasTex;

void main()
{
	//STORE THE DATA IN THE G_BUFFER
	//Fragment pos vector in the first gbuffer texture
	gPos = fragPos;
	
	//Normals
	gNorm = normalize(normal);
	
	//Diffuse  color
	if (hasTex == 0)
		gColorSpec.rgb = vec3(1, 1, 1);
	else
		gColorSpec.rgb = texture(matDiffuse1, texCoords).rgb;
	
	//Specular intensity
	if (hasTex == 0)
		gColorSpec.a = 1;
	else
		gColorSpec.a = texture(matSpecular1, texCoords).r;

}



