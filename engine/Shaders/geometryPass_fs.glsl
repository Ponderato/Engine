#version 330 core

//Control to which color buffer the fragment shader writes to.
layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec4 gColorSpec;
layout (location = 3) out vec4 gEmissive;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform sampler2D matDiffuse1;
uniform sampler2D matSpecular1;
uniform sampler2D matEmissive1;

uniform int hasTex;
uniform int hasEmissive;

void main()
{
	//STORE THE DATA IN THE G_BUFFER
	//Fragment pos vector in the first gbuffer texture
	gPos = fragPos;
	
	//Normals
	gNorm = normalize(normal);
	
	//Diffuse  color
	if (hasTex == 0){
		gColorSpec.rgb = vec3(1, 1, 1);
		gColorSpec.a = 1;
		gEmissive.rgb = vec3(0, 0, 0);
	}else{
		gColorSpec.rgb = texture(matDiffuse1, texCoords).rgb;
		gColorSpec.a = texture(matSpecular1, texCoords).r;
		if (hasEmissive == 1)
			gEmissive.rgb = texture(matEmissive1, texCoords).rgb;
		else
			gEmissive.rgb = vec3(0, 0, 0);
	}
}



