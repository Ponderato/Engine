#pragma once

#ifndef FORWARDSTEP_H
#define FORWARDSTEP_H

#include "Step.h"

class ForwardStep : public Step
{
public:

	ForwardStep(Camera* camera, Program& program);

	inline void SetModels(std::vector<Node*> models) { this->models = models; }
	inline void SetProgram(Program& program) { this->program = program; }

	void RenderStep() override;
private:

	Camera* camera;
	Program& program;

	std::vector<Node*> models;
};

#endif
