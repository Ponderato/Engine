#pragma once

#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"
#include "Program.h"

class Cube 
{
public:

	//Constructors with & without emmisive texture map
	Cube(std::string diffuse, std::string specular, std::string emissive);
	Cube(std::string diffuse, std::string specular);

	void Draw(Program &program);
private:

	float cubeVertices[72] = {
	-0.5f, -0.5f, -0.5f, //0
	 0.5f, -0.5f, -0.5f, //1
	 0.5f,  0.5f, -0.5f, //2
	-0.5f,  0.5f, -0.5f, //3

	-0.5f, -0.5f,  0.5f, //4
	 0.5f, -0.5f,  0.5f, //5
	 0.5f,  0.5f,  0.5f, //6
	-0.5f,  0.5f,  0.5f, //7

	-0.5f,  0.5f,  0.5f, //8
	-0.5f,  0.5f, -0.5f, //9
	-0.5f, -0.5f, -0.5f, //10
	-0.5f, -0.5f,  0.5f, //11

	 0.5f,  0.5f,  0.5f, //12
	 0.5f,  0.5f, -0.5f, //13
	 0.5f, -0.5f, -0.5f, //14
	 0.5f, -0.5f,  0.5f, //15

	-0.5f, -0.5f, -0.5f, //16
	 0.5f, -0.5f, -0.5f, //17
	 0.5f, -0.5f,  0.5f, //18
	-0.5f, -0.5f,  0.5f, //19

	-0.5f,  0.5f, -0.5f, //20
	 0.5f,  0.5f, -0.5f, //21
	 0.5f,  0.5f,  0.5f, //22
	-0.5f,  0.5f,  0.5f, //23
	};
	float cubeTexCoords[48] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	float cubeNormals[72] = {
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,

		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,

		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,

		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,

		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,

		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
	};
	unsigned int cubeTriangleIndex[36] = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23
	};

	Mesh* mesh;
};

#endif 
