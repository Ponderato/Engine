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

//----------------NO GLOBAL VARIABLES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const int CUBES_COUNT = 7;

static Cube cubes[CUBES_COUNT];
static Camera camera;

//Methods

void initGLEW();

void initOGL();

void initData();

void initShaders(int shaderNum, const char* vertexShaderPath, const char* fragmentShaderPath);

void render();

#endif