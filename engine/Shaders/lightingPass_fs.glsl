#version 330 core

out vec4 fColor;

in vec2 texCoords;

//Gbuffer data
uniform sampler2D gPos;
uniform sampler2D gNorm;
uniform sampler2D gColorSpec;

vec3 fragPos;
vec3 normal;
vec3 diffuse; //Albedo
float specular;

//Material properties
float matShininess = 64.0f;

//Light properties
uniform vec3 lightColor[3];
uniform vec3 lightPosition[3];

float kConst = 1.0f;     //Point light
float kLinear = 0.07f;   //Point light
float kQuad = 0.017f;    //Point light

//Other data
uniform vec3 viewerPos;

vec3 pointLShade(vec3 color, vec3 lightPos){
    //Attenuation
    float dist = length(lightPos - fragPos);
    float attenuation = 1.0 / (kConst + kLinear*dist + kQuad*dist*dist);

    //Ambient lighting.
    vec3 ambientL = color * diffuse * 0.0f; //We take here the diffuse vec3 since
    //it is obtained from the rgb components of colorSpec, which is the Albedo.
    //ambient *= attenuation;

    //Diffuse lighting.
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.1); //Ligth impact on the fragment
    vec3 diffuseL = color * diffuse * diff;
    diffuseL *= attenuation;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);//-lightDir because it expects a 
    //vector that points from the light to the frag pos and the way we calculate the 
    //substraction, it is going the oposite direction
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specularL = color * spec * specular;
    specularL *= attenuation;

    //Emissive lighting
    //vec3 emissive = texture(matEmissive, texCoords).rgb;

    return ambientL + diffuseL + specularL;
}

void main()
{
    //First get the data from the gBuffer
	fragPos = texture(gPos, texCoords).rgb;
	normal = texture(gNorm, texCoords).rgb;
	diffuse = texture(gColorSpec, texCoords).rgb;
	specular = texture(gColorSpec, texCoords).a;

    if((normal.r == 0) && (normal.g == 0) && (normal.b == 0)){  
        fColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    }else{
	    //Now light calculations as usual
        vec3 result;
        for (int i = 0; i < 3; i++){
            result += pointLShade(lightColor[i], lightPosition[i]);
        }

        fColor = vec4(result, 1.0f);
    }
}