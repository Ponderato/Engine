#include "Step.h"

void Step::RenderStep(Camera& camera, Program& program, unsigned int* buffer, std::vector<Cube>* cubes, glm::vec3 cubePositions[], std::vector<Model>* models) {}

void Step::RenderStep(Camera& camera, Program& program, unsigned int* gPos, unsigned int* gNorm, unsigned int* gColorSpec) {}

void Step::RenderStep(unsigned int* readBuffer, unsigned int* drawBuffer, GLbitfield mask, unsigned int width, unsigned int height) {}

void Step::RenderStep(unsigned int* readBuffer, GLbitfield mask, unsigned int width, unsigned int height) {}

void Step::RenderStep(Camera& camera, Program& program, std::vector<LightCube>* lightCubes) {}