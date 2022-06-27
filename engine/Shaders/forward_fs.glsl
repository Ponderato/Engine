#version 330 core 

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

layout (location = 0) out vec4 fColor;

//------------UNIFORMS------------------
//Material properties
uniform sampler2D matDiffuse1;
uniform sampler2D matDiffuse2;
uniform sampler2D matDiffuse3;

uniform sampler2D matSpecular1;
uniform sampler2D matSpecular2;
uniform sampler2D matSpecular3;

uniform sampler2D matEmissive1;
uniform sampler2D matEmissive2;
uniform sampler2D matEmissive3;

uniform sampler2D aux1;
uniform sampler2D aux2;
uniform sampler2D aux3;

uniform int hasTex;

//viewer position
uniform vec3 viewerPos;

//Light properties (WE CAN HAVE 100 LIGHTS)
uniform int nLights;
uniform float intensity[100];
uniform vec3 lightColor[100]; 
uniform vec3 lightPosition[100];

uniform int isLight;
uniform vec3 color;

uniform vec3 spotlightPos;               //Spot Light
uniform vec3 spotlightDir;               //Spot Light
//----------------------------------------------------------

float matShininess = 64.0f;

vec3 lightAmbient = vec3(0.1, 0.1, 0.1);
vec3 lightDiffuse = vec3(0.4, 0.4, 0.4);
vec3 lightSpecular = vec3(1.0, 1.0, 1.0);

vec3 lightDirection = vec3(-0.4, -1.0, -0.65); //Directional light

float kConst = 1.0f;                     //Point light
float kLinear = 0.07f;                   //Point light
float kQuad = 0.017f;                    //Point light


float cutoff = cos(radians(12.5f));      //Spot Light
float outerCutoff = cos(radians(17.5f)); //Spot Light

//There is plenty of room for optimization in these methods. Lots of duplicated
//operations.

vec3 directionalLShade(vec3 color, sampler2D matDiffuse, sampler2D matSpecular, sampler2D matEmissive){
    //Ambient lighting.
    vec3 ambient = lightAmbient * texture(matDiffuse, texCoords).rgb;

    //Diffuse lighting.
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(matDiffuse, texCoords).rgb;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//-lightDir because it expects a vector that points from the light to the frag pos.
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specular = lightSpecular * spec * texture(matSpecular, texCoords).rgb;

    //Emissive lighting
    //vec3 emissive = texture(matEmissive, texCoords).rgb;

    return (ambient + diffuse + specular) * color;
}

vec3 pointLShade(vec3 color, vec3 lightPos, sampler2D matDiffuse, sampler2D matSpecular, sampler2D matEmissive, float intensity){
    //Attenuation
    float dist = length(lightPos - fragPos);
    float attenuation = 1.0 / (kConst + kLinear*dist + kQuad*dist*dist);
    attenuation *= intensity;

    //Ambient lighting.
    vec3 ambient;
    if (hasTex == 0)
        ambient = lightAmbient * vec3(1);
    else
        ambient = lightAmbient * texture(matDiffuse, texCoords).rgb;

    //ambient *= attenuation;

    //Diffuse lighting.
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse;
    if (hasTex == 0)
        diffuse = color * diff * vec3(1);
    else
        diffuse = color * diff * texture(matDiffuse, texCoords).rgb;

    diffuse *= attenuation;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specular;
    if (hasTex == 0)
        specular = color * spec * vec3(1);
    else
        specular = color * spec * texture(matSpecular, texCoords).rgb;

    specular *= attenuation;

    //Emissive lighting
    //vec3 emissive = texture(matEmissive, texCoords).rgb;

    return (ambient + diffuse + specular);
}

vec3 spotLShade(vec3 color, sampler2D matDiffuse, sampler2D matSpecular, sampler2D matEmissive){
    vec3 lightDir = normalize(spotlightPos - fragPos);
    float theta = dot(lightDir, normalize(-spotlightDir));
    float epsilon = cutoff - outerCutoff;
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

    //Attenuation
    float dist = length(spotlightPos - fragPos);
    float attenuation = 1.0 / (kConst + kLinear*dist + kQuad*dist*dist);

    //Ambient lighting.
    vec3 ambient = lightAmbient * texture(matDiffuse, texCoords).rgb;
    ambient *= attenuation;

    //Diffuse lighting.
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(matDiffuse, texCoords).rgb;
    diffuse *= intensity;
    diffuse *= attenuation;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//-lightDir because it expects a vector that points from the light to the frag pos.
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specular = lightSpecular * spec * texture(matSpecular, texCoords).rgb;
    specular *= intensity;
    specular *= attenuation;

    //Emissive lighting
    //vec3 emissive = texture(matEmissive, texCoords).rgb;

    return ambient + diffuse + specular * color;
}

void main(){
    
    if (isLight == 1){
        fColor = vec4(color, 1.0f);
    }
    else{
        vec3 result;
        for (int i = 0; i < nLights; i++){
            result += pointLShade(lightColor[i], lightPosition[i], matDiffuse1, matSpecular1, matEmissive1, intensity[i]);
        }

        fColor = vec4(result, 1.0f);
        
    }
}