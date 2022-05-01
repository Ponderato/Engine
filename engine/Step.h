#pragma once

#ifndef STEP_H
#define STEP_H

#include <GL/glew.h>
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <ext/matrix_clip_space.hpp>

#include "Camera.h"
#include "Cube.h"
#include "LightCube.h"
#include "Model.h"

class Step 
{
public:

	//Step() {};

	virtual void RenderStep(unsigned int& inBuffer, unsigned int& outBuffer) = 0;
protected:

	glm::mat4 view_M = glm::mat4(1.0f);
	glm::mat4 proj_M = glm::mat4(1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 normal = glm::mat4(1.0f);
};

#endif 
