#include "LightCube.h"

LightCube::LightCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, glm::vec3 color) : Cube(position, scale, rotation) {

	this->color = color;
}