#version 330 core

//in vec3 color;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float faceV;

out vec4 fcolor;

void main(){
    //fcolor = vec4(color, 1.0f);
    //fcolor = texture(texture1, texCoord) * vec4(color, 1.0);
    fcolor = mix(texture(texture1, texCoord), texture(texture2, vec2(-texCoord.x, texCoord.y)), faceV);
    
}