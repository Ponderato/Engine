#pragma once

#ifndef GEOMETRYSTEP_H
#define GEOMETRYSTEP_H

#include "Step.h"

class GeometryStep : public Step
{
public:

	GeometryStep(Camera& camera, Program& program, std::vector<Model*> models);

	void AttachTextures();
	void SetUp_gBuffer(const unsigned int WIDTH, const unsigned int HEIGHT);

	void RenderStep() override;

private:

	Camera& camera;
	Program& program;

	std::vector<Model*> models;
};


#endif