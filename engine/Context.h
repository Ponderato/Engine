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

	std::vector<Node*> nodes;//Models and cubes all combined so we can show them

	Context() = default;

	inline void SetWIdth(const unsigned int WIDTH) { this->WIDTH = WIDTH; }
	inline void SetHeight(const unsigned int HEIGHT) { this->HEIGHT = HEIGHT; }

	void InitGLEW();
	void InitOGL();

	void InitCube(std::string diffuse, std::string specular, std::string emissive, glm::vec3 position, Node* node);
	void InitCube(std::string diffuse, std::string specular, glm::vec3 position, Node* node);
	void InitCube(glm::vec3 position, Node* node);
	void InitLightCube(glm::vec3 position, glm::vec3 color, Node* node);
	void InitModel(const std::string& path, glm::vec3 position, Node* node);

	void InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
	void InitCamera(const glm::vec3 pos, const glm::vec3 worldUp, const float speed, const float sensitivity, const float fov, const float yaw, const float pitch, Node* parent);

	void SetPipeline();
	void SetLightUniforms(Program& program, int nLights, glm::vec3 lightColor[], glm::vec3 lightPos[]);

	inline unsigned int GetRenderTexture() { return this->renderTexture; };

	void Update();
private:

	//Framebuffers
	unsigned int defaultBuffer;
	unsigned int gBuffer;
	unsigned int middleBuffer;

	//Gbuffer data (Texture Id's)
	unsigned int gPos, gNorm, gColorSpec;
	unsigned int renderTexture;

	std::vector<Node*> renderableModels;
	std::vector<Node*> renderableForwardModels;

	void SetUniforms();

	void CheckRenderable();
	void UpdateModels();

	void GetFrameBufferID(unsigned int *framebuffer);
};

#endif