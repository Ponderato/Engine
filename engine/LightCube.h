#pragma once

#ifndef LIGHTCUBE_H
#define LIGHTCUBE_H

#include "Cube.h"

class LightCube : public Cube
{
public:

	float intensity;
	glm::vec3 color = glm::vec3(0);

	LightCube(glm::vec3 position, glm::vec3 color, Node* parent);

	inline void SetIntensity(float intensity) { this->intensity = intensity; }
	inline float GetIntensity() { return this->intensity; }
};

#endif