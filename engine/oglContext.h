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

class Context{
public:
	//----------------NO GLOBAL VARIABLES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Camera camera;

	//Methods

	void initGLEW();

	void initOGL();

	void initData();

	void initShaders(int shaderNum, const char* vertexShaderPath, const char* fragmentShaderPath);

	void render();

private:


};


#endif