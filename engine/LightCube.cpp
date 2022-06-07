#include "LightCube.h"

LightCube::LightCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, glm::vec3 color, Node* parent) : Cube(position, scale, rotation, parent) {

	this->color = color;
	this->forward = true;
	this->tag = "LightCube";
}