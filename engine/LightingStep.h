#pragma once

#ifndef LIGHTINGSTEP_H
#define LIGHTINGSTEP_H

#include "Step.h"

class LightingStep : public Step
{
public:

	LightingStep() {};

	void RenderStep(Camera& camera, Program& program, unsigned int* gPos, unsigned int* gNorm, unsigned int* gColorSpec) override;
private:

	//QUAD data
	unsigned int quadVAO = 0;
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
};


#endif