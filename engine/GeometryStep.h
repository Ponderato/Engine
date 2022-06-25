#pragma once

#ifndef GEOMETRYSTEP_H
#define GEOMETRYSTEP_H

#include "Step.h"

class GeometryStep : public Step
{
public:

	GeometryStep(Camera* camera, Program& program);

	inline void SetModels(std::vector<Node*> models) { this->models = models; }
	inline void SetProgram(Program& program) { this->program = program; }

	void RenderStep() override;
private:

	Camera* camera;
	Program& program;

	std::vector<Node*> models;
};


#endif