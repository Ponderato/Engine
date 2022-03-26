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

const int CUBES_COUNT = 7;
const int SHADERS_COUNT = 2;

Program programs[CUBES_COUNT];
Cube cubes[SHADERS_COUNT];
Camera* camera;

//Methods

void initGLEW();

void initOGL();

void initData();

void initShaders(int shaderNum, const char* vertexShaderPath, const char* fragmentShaderPath);

void render();

#endif