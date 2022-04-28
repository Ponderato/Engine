#pragma once

#ifndef GEOMETRYSTEP_H
#define GEOMETRYSTEP_H

#include "Step.h"

class GeometryStep : public Step
{
public:

	GeometryStep() {};

	void RenderStep(Camera& camera, Program& program, unsigned int* gBuffer, std::vector<Cube>* cubes, glm::vec3 cubePositions[], std::vector<Model>* models) override;
};


#endif