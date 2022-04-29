#include "LightCube.h"

LightCube::LightCube(glm::vec3 position, glm::vec3 color) : Cube(position) {

	this->color = color;
}