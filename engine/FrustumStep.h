#pragma once

#ifndef FRUSTUMSTEP_H
#define FRUSTUMSTEP_H

#include "Step.h"

class FrustumStep : public Step
{
public:

	FrustumStep(Camera* camera, Program& program);

	inline void SetInactiveCameras(std::vector<Camera*> inactiveCameras) { this->inactiveCameras = inactiveCameras; }
	inline void SetProgram(Program& program) { this->program = program; }
	inline void SetCamera(Camera* camera) { this->camera = camera; }

	void RenderStep() override;

private:

	Camera* camera;
	Program& program;
	std::vector<Camera*> inactiveCameras;

	void DrawCamerasFrustum();
};


#endif