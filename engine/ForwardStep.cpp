#include "ForwardStep.h"

ForwardStep::ForwardStep(Camera& camera, Program& program, std::vector<LightCube>& cubes) 
	: camera(camera), program(program), cubes(cubes){

}

void ForwardStep::RenderStep(unsigned int& inBuffer, unsigned int& outBuffer) {

	glBindFramebuffer(GL_FRAMEBUFFER, inBuffer);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view_M = camera.GetLookAtMatrix();
	proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	program.Use();
	program.SetMat4("projM", proj_M);
	program.SetMat4("viewM", view_M); 

	for (int i = 0; i < cubes.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubes.at(i).position);
		model = glm::scale(model, cubes.at(i).scale);
		program.SetMat4("modelM", model);
		program.SetVec3("lightColor", cubes.at(i).color);
		cubes.at(i).Draw(program);
	}
}