#include "LightCube.h"

LightCube::LightCube(glm::vec3 position, glm::vec3 color, glm::vec3 scale) : Cube(position, scale) {

	this->color = color;
}