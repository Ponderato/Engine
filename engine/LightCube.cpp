#include "LightCube.h"

LightCube::LightCube(glm::vec3 position, glm::vec3 scale, glm::vec3 color) : Cube(position, scale) {

	this->color = color;
}