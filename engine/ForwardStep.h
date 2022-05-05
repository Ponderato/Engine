#pragma once

#ifndef FORWARDSTEP_H
#define FORWARDSTEP_H

#include "Step.h"

class ForwardStep : public Step
{
public:

	ForwardStep(Camera& camera, Program& program, std::vector<Model*> models);

	void RenderStep() override;
private:

	Camera& camera;
	Program& program;

	std::vector<Model*> models;

};

#endif
