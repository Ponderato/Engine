#version 330 core 

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 viewerPos;

out vec4 fColor;

//Material properties
uniform sampler2D matDiffuse1;
uniform sampler2D matSpecular1;
uniform sampler2D matEmissive11;
float matShininess = 64.0f;

//Light properties
uniform vec3 lightColor[3];
uniform vec3 lightPosition[3];

vec3 lightAmbient = vec3(0.1, 0.1, 0.1);
vec3 lightDiffuse = vec3(0.4, 0.4, 0.4);
vec3 lightSpecular = vec3(1.0, 1.0, 1.0);

vec3 lightDirection = vec3(-0.4, -1.0, -0.65); //Directional light

float kConst = 1.0f;                     //Point light
float kLinear = 0.07f;                   //Point light
float kQuad = 0.017f;                    //Point light

uniform vec3 spotlightPos;               //Spot Light
uniform vec3 spotlightDir;               //Spot Light
float cutoff = cos(radians(12.5f));      //Spot Light
float outerCutoff = cos(radians(17.5f)); //Spot Light

//There is plenty of room for optimization in these methods. Lots of duplicated
//operations.
vec3 directionalLShade(vec3 color){
    //Ambient lighting.
    vec3 ambient = lightAmbient * texture(matDiffuse1, texCoords).rgb;

    //Diffuse lighting.
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(matDiffuse1, texCoords).rgb;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//-lightDir because it expects a vector that points from the light to the frag pos.
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specular = lightSpecular * spec * texture(matSpecular1, texCoords).rgb;

    //Emissive lighting
    //vec3 emissive = texture(matEmissive, texCoords).rgb;

    return (ambient + diffuse + specular) * color;
}

vec3 pointLShade(vec3 color, vec3 lightPos){
    //Attenuation
    float dist = length(lightPos - fragPos);
    float attenuation = 1.0 / (kConst + kLinear*dist + kQuad*dist*dist);

    //Ambient lighting.
    vec3 ambient = lightAmbient * texture(matDiffuse1, texCoords).rgb;
    ambient *= attenuation;

    //Diffuse lighting.
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(matDiffuse1, texCoords).rgb;
    diffuse *= attenuation;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//-lightDir because it expects a vector that points from the light to the frag pos and the way we calculate the 
    //substraction, it is going the oposite direction
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specular = lightSpecular * spec * texture(matSpecular1, texCoords).rgb;
    specular *= attenuation;

    //Emissive lighting
    //vec3 emissive = texture(matEmissive, texCoords).rgb;

    return (ambient + diffuse + specular) * color;
}

vec3 spotLShade(vec3 color){
    vec3 lightDir = normalize(spotlightPos - fragPos);
    float theta = dot(lightDir, normalize(-spotlightDir));
    float epsilon = cutoff - outerCutoff;
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

    //Attenuation
    float dist = length(spotlightPos - fragPos);
    float attenuation = 1.0 / (kConst + kLinear*dist + kQuad*dist*dist);

    //Ambient lighting.
    vec3 ambient = lightAmbient * texture(matDiffuse1, texCoords).rgb;
    ambient *= attenuation;

    //Diffuse lighting.
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(matDiffuse1, texCoords).rgb;
    diffuse *= intensity;
    diffuse *= attenuation;

    //Specular lighting
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//-lightDir because it expects a vector that points from the light to the frag pos.
    float spec = pow(max(dot(viewerDir, reflectDir), 0.0), matShininess);//last value refers to the shininess
    vec3 specular = lightSpecular * spec * texture(matSpecular1, texCoords).rgb;
    specular *= intensity;
    specular *= attenuation;

    //Emissive lighting
    //vec3 emissive = texture(matEmissive, texCoords).rgb;

    return ambient + diffuse + specular * color;
}

void main(){
    
    vec3 result = pointLShade(lightColor[0], lightPosition[0]);
    result += pointLShade(lightColor[1], lightPosition[1]);
    result += pointLShade(lightColor[2], lightPosition[2]);

    fColor = vec4(result, 1.0);
}