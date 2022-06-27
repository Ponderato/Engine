#version 330 core

layout (location = 3) out vec4 fColor; //renderTex

in vec2 texCoords;

//--------------UNIFORMS------------------
//Gbuffer data
uniform sampler2D gPos;
uniform sampler2D gNorm;
uniform sampler2D gColorSpec;
uniform sampler2D gEmissive;

//Aux data
uniform sampler2D aux1;
uniform sampler2D aux2;
uniform sampler2D aux3;

//Light properties (WE CAN HAVE 100 LIGHTS)
uniform int nLights;
uniform float intensity[100];
uniform vec3 lightColor[100]; 
uniform vec3 lightPosition[100];

//Other data
uniform vec3 viewerPos;

//-----------------------------------------

vec3 fragPos;
vec3 normal;
vec3 diffuse; //Albedo
float specular;

//Material properties
float matShininess = 64.0f;

float kConst = 1.0f;     //Point light
float kLinear = 0.07f;   //Point light
float kQuad = 0.017f;    //Point light

vec3 pointLShade(vec3 color, vec3 lightPos, float intensity){
    //Attenuation
    float dist = length(lightPos - fragPos);
    float attenuation = 1.0 / (kConst + kLinear*dist + kQuad*dist*dist);
    attenuation *= intensity;

    //Ambient lighting.
    //vec3 ambientL = color * diffuse * 0.05f; //We take here the diffuse vec3 since
    //it is obtained from the rgb components of colorSpec, which is the Albedo.
    //ambient *= attenuation;

    //Diffuse lighting.
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.1); //Ligth impact on the fragment
    vec3 diffuseL = color * diffuse * diff;
    diffuseL *= attenuation;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specularL = color * spec * specular;
    specularL *= attenuation;

    //Emissive lighting
    //vec3 emissive = texture(matEmissive, texCoords).rgb;

    return  diffuseL + specularL;
    //return result * intensity;
}

void main()
{
    //First get the data from the gBuffer
	fragPos = texture(gPos, texCoords).rgb;
	normal = texture(gNorm, texCoords).rgb;
	diffuse = texture(gColorSpec, texCoords).rgb;
	specular = texture(gColorSpec, texCoords).a;

	//Now light calculations as usual
    vec3 result;
    for (int i = 0; i < nLights; i++){
        result += pointLShade(lightColor[i], lightPosition[i], intensity[i]);
    }

    fColor = vec4(result, 1.0f);
}