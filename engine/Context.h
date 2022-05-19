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

	float deltaTime;

	Camera camera;
	Pipeline* pipeline;
	Node* parentNode = new Node();
	std::vector<Program> programs;

	unsigned int WIDTH;
	unsigned int HEIGHT;

	std::vector<Model*> models;//Models and cubes all combined in one vector so in the geometry step we do only one for loop

	Context();

	inline void SetWIdth(const unsigned int WIDTH) { this->WIDTH = WIDTH; }
	inline void SetHeight(const unsigned int HEIGHT) { this->HEIGHT = HEIGHT; }

	void InitGLEW();
	void InitOGL();

	void InitCube(std::string diffuse, std::string specular, std::string emissive, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* node);
	void InitCube(std::string diffuse, std::string specular, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* node);
	void InitCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* node);
	void InitLightCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, glm::vec3 color, Node* node);
	void InitModel(const std::string& path, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* node);

	void InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
	void InitCamera(const glm::vec3 pos, const glm::vec3 worldUp, const float speed, const float sensitivity, const float fov, const float yaw, const float pitch);

	void SetUniforms();
	void SetDefaultPipeline();

	inline void SetProjectionMatrix(float near, float far) { this->projM = glm::perspective(glm::radians(camera.fov), (float)WIDTH / HEIGHT, near, far); };

	void Update();
private:

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

	glm::mat4 projM;

	//Framebuffers
	unsigned int defaultFBuffer = 0;
	unsigned int gBuffer;

	//Gbuffer data (Texture Id's)
	unsigned int gPos, gNorm, gColorSpec;

	std::vector<Model*> renderableModels;
	std::vector<Model*> renderableForwardModels;

	void CheckRenderable();
	void UpdateModels();
};

#endif