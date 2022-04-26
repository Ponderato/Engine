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

void main()
{
	//Store the data in the gbuffer

	//Fragment pos vector in the first gbuffer texture
	gPos = fragPos;
	
	//Normals
	gNorm = normalize(normal);
	
	//Diffuse  color
	gColorSpec.rgb = texture(matDiffuse1, texCoords).rgb;
	
	//Specular intensity
	gColorSpec.a = texture(matSpecular1, texCoords).r;

}



