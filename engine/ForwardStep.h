#pragma once

#ifndef FORWARDSTEP_H
#define FORWARDSTEP_H

#include "Step.h"

class ForwardStep : public Step
{
public:

	void RenderStep(Camera& camera, Program& program, std::vector<LightCube>* lightCubes) override;
};

#endif
