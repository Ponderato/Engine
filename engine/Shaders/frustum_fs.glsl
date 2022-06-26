#version 330 core 

layout (location = 0) out vec4 fColor;

uniform vec3 color;

void main(){
    
    fColor = vec4(color, 1.0);
}