#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <string>
#include <vector>
#include <iostream>

#include "Mesh.h"

class Model
{
public:
	
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	virtual void Draw(Program& program) = 0;
};

#endif