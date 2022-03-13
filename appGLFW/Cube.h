#pragma once

#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"
#include "Program.h"

class Cube 
{
public:
	Cube(float verts[], float texCoords[], float normals[], const unsigned int inds[], std::string diffuse, std::string specular, std::string emissive);

	void Draw(Program &program);

private:
	Mesh* mesh;

};

#endif 
