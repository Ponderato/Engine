#pragma once

#ifndef LIGHTCUBE_H
#define LIGHTCUBE_H

#include "Cube.h"

class LightCube : public Cube
{
public:

	LightCube(glm::vec3 position, glm::vec3 color, Node* parent);

	inline void SetIntensity(float intensity) { this->intensity = intensity; }
	inline void SetColor(glm::vec3 color) { this->color = color; }

	inline float GetIntensity() { return this->intensity; }
	inline glm::vec3 GetColor() { return this->color; }
private:

	float intensity;
	glm::vec3 color = glm::vec3(0);
};

#endif