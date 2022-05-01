#pragma once

#ifndef FORWARDSTEP_H
#define FORWARDSTEP_H

#include "Step.h"

class ForwardStep : public Step
{
public:

	ForwardStep(Camera& camera, Program& program, std::vector<LightCube>& cubes);

	void RenderStep(unsigned int& inBuffer, unsigned int& outBuffer) override;
private:

	Camera& camera;
	Program& program;

	std::vector<LightCube>& cubes;

};

#endif
