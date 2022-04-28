#pragma once

#ifndef OGLCONTEXT_H
#define OGLCONTEXT_H

#include <GL/glew.h>
#include <glm.hpp>

#include <iostream>
#include <stdio.h>

#include "Program.h"
#include "Cube.h"
#include "Camera.h"
#include "Model.h"

#include "Step.h"
#include "GeometryStep.h"
#include "LightingStep.h"
#include "CopyStep.h"
#include "ForwardStep.h"

class Context
{
public:
	Camera camera;

	Context();

	void InitGLEW();

	void InitOGL();

	void InitData();

	void InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath);

	void Render();

private:

	const unsigned int WIDTH = 800;
	const unsigned int HEIGHT = 600;

	//Matrices
	glm::mat4 view_M;
	glm::mat4 proj_M = glm::mat4(1.0f);

	//Light & cube data
	glm::vec3 lightPos[3] = {
	glm::vec3(2.0f, 2.0f, 0.0f),
	glm::vec3(-2.0f, -3.0f, -4.0f),
	glm::vec3(1.5f, 1.0f, -6.0f)
	};
	glm::vec3 lightColor[3] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};
	glm::vec3 cubePositions[7] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(1.5f,  1.0f,  0.0f),
		glm::vec3(-1.0f,  1.0f,  -1.5f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-2.5f, -4.2f, -4.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -1.4f, -3.5f)
	};

	//Gbuffer data
	unsigned int gBuffer;
	unsigned int gPos, gNorm, gColorSpec;

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

	//Vectors
	std::vector<Program> programs;
	std::vector<Cube> cubes;
	std::vector<Model> models;
	std::vector<Cube> lightCubes;

	//Steps
	Step fatherStep;
	GeometryStep gStep;
	LightingStep lStep;
	CopyStep cStep;
	ForwardStep fStep;

	//Methods
	void ConfigureG_Buffer();
	void RenderQuad();
};

#endif