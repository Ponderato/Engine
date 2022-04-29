#include "ForwardStep.h"

void ForwardStep::RenderStep(Camera& camera, Program& program, std::vector<LightCube>* lightCubes) {

	view_M = camera.GetLookAtMatrix();
	proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	program.Use();
	program.SetMat4("projM", proj_M);
	program.SetMat4("viewM", view_M); 

	for (int i = 0; i < lightCubes->size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightCubes->at(i).position);
		model = glm::scale(model, glm::vec3(0.25f));
		program.SetMat4("modelM", model);
		program.SetVec3("lightColor", lightCubes->at(i).color);
		lightCubes->at(i).Draw(program);
	}
}