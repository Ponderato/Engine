#pragma once

#ifndef LIGHTCUBE_H
#define LIGHTCUBE_H

#include "Cube.h"

class LightCube : public Cube
{
public:

	glm::vec3 color = glm::vec3(0);

	LightCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, glm::vec3 color);
};

#endif