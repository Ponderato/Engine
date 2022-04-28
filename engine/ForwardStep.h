#pragma once

#ifndef FORWARDSTEP_H
#define FORWARDSTEP_H

#include "Step.h"

class ForwardStep : public Step
{
public:

	void RenderStep(Camera& camera, Program& program, std::vector<Cube>* lightCubes, const glm::vec3 positions[], const glm::vec3 colors[]) override;
};

#endif
