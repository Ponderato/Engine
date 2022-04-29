#pragma once

#ifndef SCENEDATA_H
#define SCENEDATA_H

#include <vector>

#include "Cube.h"
#include "LightCube.h"
#include "Model.h"


class SceneData
{
public:
	
	std::vector<Cube> cubes;
	std::vector<Model> models;

	std::vector<LightCube> lightCubes;

};

#endif