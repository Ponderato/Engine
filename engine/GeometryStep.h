#pragma once

#ifndef GEOMETRYSTEP_H
#define GEOMETRYSTEP_H

#include "Step.h"

class GeometryStep : public Step
{
public:

	GeometryStep(Camera& camera, Program& program, std::vector<Cube>& cubes, std::vector<AssimpModel>& models);

	void RenderStep(unsigned int& inBuffer, unsigned int& outBuffer) override;

private:

	Camera& camera;
	Program& program;

	std::vector<Cube>& cubes;
	std::vector<AssimpModel>& models;
};


#endif