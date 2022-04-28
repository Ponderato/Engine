#pragma once

#ifndef STEP_H
#define STEP_H

#include <GL/glew.h>
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/matrix_clip_space.hpp>

#include "Camera.h"
#include "Cube.h"
#include "Model.h"

class Step 
{
public:

	Step() {};

	virtual void RenderStep(Camera& camera, Program& program, unsigned int* buffer, std::vector<Cube>* cubes, glm::vec3 cubePositions[], std::vector<Model>* models);
	virtual void RenderStep(Camera& camera, Program& program, unsigned int* gPos, unsigned int* gNorm, unsigned int* gColorSpec);
	virtual void RenderStep(unsigned int* readBuffer, unsigned int* drawBuffer, GLbitfield mask, unsigned int width, unsigned int height);	
	virtual void RenderStep(Camera& camera, Program& program, std::vector<Cube>* lightCubes, const glm::vec3 positions[], const glm::vec3 colors[]);
protected:

	glm::mat4 view_M = glm::mat4(1.0f);
	glm::mat4 proj_M = glm::mat4(1.0f);

};

#endif 
