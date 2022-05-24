#pragma once

#ifndef GEOMETRYSTEP_H
#define GEOMETRYSTEP_H

#include "Step.h"

class GeometryStep : public Step
{
public:

	GeometryStep(Camera& camera, Program& program);

	inline void SetModels(std::vector<Model*> models) { this->models = models; }
	inline void SetProjectionMatrix(glm::mat4 projM) { this->projM = projM; }

	void RenderStep() override;
private:

	Camera& camera;
	Program& program;

	std::vector<Model*> models;

	glm::mat4 projM;
};


#endif