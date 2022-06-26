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
	inline void SetCamera(Camera* camera) { this->camera = camera; }

	inline void SetInactiveCameras(std::vector<Camera*> inactiveCameras) { this->inactiveCameras = inactiveCameras; }

	void RenderStep() override;
private:

	Camera* camera;
	Program& program;

	std::vector<Camera*> inactiveCameras;

	std::vector<Node*> models;

	void DrawCamerasFrustum();
};

#endif
