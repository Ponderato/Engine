#version 330 core 

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 viewerPos;

out vec4 fColor;

//Material properties
uniform sampler2D matDiffuse;
uniform sampler2D matSpecular;
uniform sampler2D matEmissive;
float matShininess = 64.0f;

//Light properties
uniform vec3 lightPos;

vec3 lightAmbient = vec3(0.1, 0.1, 0.1);
vec3 lightDiffuse = vec3(0.4, 0.4, 0.4);
vec3 lightSpecular = vec3(1.0, 1.0, 1.0);

vec3 lightDir = vec3(-0.4, -1.0, -0.65); //Directional light

float kConst = 1.0f;                     //Point light
float kLinear = 0.07f;                   //Point light
float kQuad = 0.017f;                    //Point light

void main(){
    //Attenuation
    float dist = length(lightPos - fragPos);
    float attenuation = 1.0 / (kConst + kLinear*dist + kQuad*dist*dist);

    //Ambient lighting.
    vec3 ambient = lightAmbient * texture(matDiffuse, texCoords).rgb;
    ambient *= attenuation;

    //Diffuse lighting.
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    //vec3 lightDir = normalize(-lightDir);//Directional Light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(matDiffuse, texCoords).rgb;
    diffuse *= attenuation;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//-lightDir because it expects a vector that points from the light to the frag pos and the way we calculate the 
    //substraction, it is going the oposite direction
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specular = lightSpecular * spec * texture(matSpecular, texCoords).rgb;
    specular *= attenuation;

    //Emissive lighting
    vec3 emissive = texture(matEmissive, texCoords).rgb;

    //fColor = vec4((ambient + diffuse + specular + emissive), 1.0);
    fColor = vec4((ambient + diffuse + specular), 1.0);
}