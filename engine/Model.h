#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <string>
#include <iostream>

#include "Mesh.h"
#include "Node.h"

class Model : public Node
{
public:

	Model();

	virtual void Draw(Program& program) = 0;
};

#endif