#pragma once

#ifndef OGLCONTEXT_H
#define OGLCONTEXT_H

#include <GL/glew.h>
#include <glm.hpp>

#include <iostream>
#include <stdio.h>

#include "Program.h"
#include "Cube.h"
#include "LightCube.h"
#include "Camera.h"
#include "AssimpModel.h"


#include "GeometryStep.h"
#include "LightingStep.h"
#include "CopyStep.h"
#include "ForwardStep.h"
#include "Pipeline.h"

class Context
{
public:
	Camera camera;

	Context();

	void InitGLEW();

	void InitOGL();

	void InitData();

	void InitCube(std::string diffuse, std::string specular, std::string emissive, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* node);
	void InitCube(std::string diffuse, std::string specular, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);
	void InitCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);
	void InitLightCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, glm::vec3 color);
	void InitModel(const std::string& path, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);

	void InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath);

	void Render();

private:

	const unsigned int WIDTH = 800;
	const unsigned int HEIGHT = 600;

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

	//Framebuffers
	unsigned int defaultFBuffer = 0;
	unsigned int gBuffer;

	//Gbuffer data (Texture Id's)
	unsigned int gPos, gNorm, gColorSpec;

	//Vectors
	std::vector<Program> programs;
	std::vector<Model*> cubes;
	std::vector<Model*> models;
	std::vector<Model*> lightCubes;

	//Pipeline
	Pipeline* pipeline;

};

#endif