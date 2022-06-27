#pragma once

#ifndef FORWARDSTEP_H
#define FORWARDSTEP_H

#include "Step.h"

class ForwardStep : public Step
{
public:

	ForwardStep(Camera* camera, Program& program, std::string activePipe);

	inline void SetModels(std::vector<Node*> models) { this->models = models; }
	inline void SetProgram(Program& program) { this->program = program; }
	inline void SetCamera(Camera* camera) { this->camera = camera; }

	void RenderStep() override;
private:

	std::string activePipe;

	Camera* camera;
	Program& program;

	std::vector<Node*> models;
	void SetLightUniforms();
};

#endif
