#include "GeometryStep.h"

GeometryStep::GeometryStep(Camera& camera, Program& program, std::vector<Cube>& cubes, std::vector<Model>& models)
	: camera(camera), program(program), cubes(cubes), models(models) {

}

void GeometryStep::RenderStep(unsigned int& inBuffer, unsigned int& outBuffer) {

	glBindFramebuffer(GL_FRAMEBUFFER, inBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view_M = camera.GetLookAtMatrix();
	proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	program.Use();
	program.SetMat4("projM", proj_M);
	program.SetMat4("viewM", view_M);

	for (int i = 0; i < cubes.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubes.at(i).position);
		normal = glm::transpose(inverse(model));
		program.SetMat4("modelM", model);
		program.SetMat4("normalM", normal);
		cubes.at(i).Draw(program);
	}

	for (int i = 0; i < models.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, models.at(i).position);
		model = glm::scale(model, models.at(i).scale);
		normal = glm::transpose(glm::inverse(model));
		program.SetMat4("modelM", model);
		program.SetMat4("normalM", normal);
		models.at(i).Draw(program);
	}
}