#version 330 core 

float ambientStrength = 0.2f;
float specularStrength = 0.5f;

in vec3 normal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

out vec4 fColor;

void main(){
    //Ambient lighting.
    vec3 ambient = ambientStrength * lightColor;

    //Diffuse lighting.
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//-lightDir because it expects a vector that points from the light to the frag pos and the way we calculate the 
    //substraction, it is going the oposite direction
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), 32);//last value refers to the shininess
    vec3 specular = specularStrength * spec * lightColor;

    fColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}