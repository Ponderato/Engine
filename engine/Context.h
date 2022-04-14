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

class Context
{
public:
	Camera camera;

	Context();

	void initGLEW();

	void initOGL();

	void initData();

	void initShaders(const char* vertexShaderPath, const char* fragmentShaderPath);

	void render();

private:
	
	std::vector<Program> programs;
	std::vector<Cube> cubes;
	std::vector<Model> models;
	std::vector<Cube> lightCubes;

	const glm::vec3 lightPos[3] = {
	glm::vec3(2.0f, 2.0f, 0.0f),
	glm::vec3(-2.0f, -3.0f, -4.0f),
	glm::vec3(1.5f, 1.0f, -6.0f)
	};
	const glm::vec3 lightColor[3] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};
	const glm::vec3 cubePositions[5] = {
		glm::vec3(-1.0f,  1.0f,  -1.5f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-2.5f, -4.2f, -4.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -1.4f, -3.5f)
	};

	glm::mat4 view_M;
	glm::mat4 proj_M = glm::mat4(1.0f);
	glm::mat4 normal_M;
};

#endif