#pragma once

#ifndef LIGHTINGSTEP_H
#define LIGHTINGSTEP_H

#include "Step.h"

class LightingStep : public Step
{
public:

	LightingStep(Camera* camera, Program& program);

	inline void SetModels(std::vector<Node*> models) { this->models = models; }
	inline void SetProgram(Program& program) { this->program = program; }
	inline void SetCamera(Camera* camera) { this->camera = camera; }

	void RenderStep() override;
private:

	Camera* camera;
	Program& program;
	std::vector<Node*> models;

	//QUAD data
	unsigned int quadVAO;
	unsigned int quadVBO;
	//Vertex + texCoords
	const float quadVertices[20] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	//Methods
	void RenderQuad();


	void SetLightUniforms();
};


#endif