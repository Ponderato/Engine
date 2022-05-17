#pragma once

#ifndef STEP_H
#define STEP_H

#include <GL/glew.h>

#include "Camera.h"
#include "Cube.h"
#include "LightCube.h"
#include "AssimpModel.h"

class Step 
{
public:

	inline void SetFBO(unsigned int* FBO) { this->FBO = FBO; };
	inline void SetInputTexture(unsigned int id, GLuint* texture) { dataTextures.insert(dataTextures.begin() + id, texture); };

	inline GLuint* GetOutputTexture(unsigned int id) { return dataTextures.at(id); };

	virtual void RenderStep() = 0;
protected:

	unsigned int* FBO;

	std::vector<GLuint*> dataTextures;

	glm::mat4 view_M = glm::mat4(1.0f);
	glm::mat4 proj_M = glm::mat4(1.0f);
};

#endif 
