#include "GeometryStep.h"

void GeometryStep::RenderStep(Camera& camera, Program& program, unsigned int* gBuffer, std::vector<Cube>* cubes, glm::vec3 cubePositions[], std::vector<Model>* models) {

	glBindFramebuffer(GL_FRAMEBUFFER, *gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view_M = camera.GetLookAtMatrix();
	proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	program.Use();
	program.SetMat4("projM", proj_M);
	program.SetMat4("viewM", view_M);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 normal = glm::mat4(1.0f);

	for (int i = 0; i < cubes->size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		normal = glm::transpose(inverse(model));
		program.SetMat4("modelM", model);
		program.SetMat4("normalM", normal);
		cubes->at(i).Draw(program);
	}

	for (int i = 0; i < models->size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f));
		normal = glm::transpose(glm::inverse(model));
		program.SetMat4("modelM", model);
		program.SetMat4("normalM", normal);
		models->at(i).Draw(program);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}