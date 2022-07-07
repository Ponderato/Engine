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
#include "FrustumStep.h"
#include "Pipeline.h"

class Context
{
public:

	Node* parentNode = new Node();
	std::vector<Program> programs;
	std::vector<Pipeline*> pipelines;
	std::vector<Node*> nodes;//Models and cubes all combined so we can show them

	Context() = default;

	void InitGLEW();
	void InitOGL();

	void InitCube(std::string diffuse, std::string specular, std::string emissive, glm::vec3 position, Node* node);
	void InitCube(std::string diffuse, std::string specular, glm::vec3 position, Node* node);
	void InitCube(std::string diffuse, glm::vec3 position, Node* node);
	void InitCube(glm::vec3 position, Node* node);
	void InitLightCube(glm::vec3 position, glm::vec3 color, Node* node);
	void InitModel(const std::string& path, glm::vec3 position, Node* node);

	void InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
	void InitCamera(const glm::vec3 pos, const glm::vec3 worldUp, const float aspect, const float speed, const float sensitivity, const float fov, const float yaw, const float pitch, Node* parent);
	void InitCameraSet(const glm::vec3 pos, const glm::vec3 worldUp, const float aspect, const float speed, const float sensitivity, const float fov, const float yaw, const float pitch, Node* parent);

	void SetDeferredPipeline();
	void SetForwardPipeline();

	void DeleteNode(Node* node);
	void DeleteChildNodes(Node* node);

	void SetActiveCamera(Camera* camera);

	inline Camera* GetActiveCamera() { return this->camera; };
	inline unsigned int GetRenderTextureD() { return this->renderTextureD; };
	inline unsigned int GetRenderTextureF() { return this->renderTextureF; };
	inline Pipeline* GetActivePipe() { return this->activePipe; };
	inline unsigned int GetWidth() { return this->WIDTH; };
	inline unsigned int GetHeight() { return this->HEIGHT; };
	inline std::vector<Camera*> GetInactiveCameras() { return this->inactiveCameras; }

	inline void SetActivePipeline(Pipeline* pipe) { this->activePipe = pipe; };
	inline void SetWIdth(const unsigned int WIDTH) { this->WIDTH = WIDTH; }
	inline void SetHeight(const unsigned int HEIGHT) { this->HEIGHT = HEIGHT; }

	void Update();
private:

	float deltaTime;

	unsigned int WIDTH;
	unsigned int HEIGHT;

	//Framebuffers deferred
	unsigned int defaultBuffer;
	unsigned int gBuffer;
	unsigned int middleBuffer;

	//Gbuffer data (Texture Id's)
	unsigned int gPos, gNorm, gColorSpec, gEmissive;
	unsigned int renderTextureD;

	//Forward
	unsigned int fbuffer;
	unsigned int renderTextureF;

	std::vector<Node*> renderableModels;
	std::vector<Node*> renderableForwardModels;

	Pipeline* activePipe;

	Camera* camera; //Active camera
	std::vector<Camera*> inactiveCameras;

	void SetDSUniforms();

	void CheckRenderable();
	void UpdateModels();

	void UpdateCameras();

	void GetFrameBufferID(unsigned int *framebuffer);
};

#endif