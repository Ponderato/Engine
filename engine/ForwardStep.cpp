#include "ForwardStep.h"

ForwardStep::ForwardStep(Camera& camera, Program& program, std::vector<Model*> models) 
	: camera(camera), program(program), models(models){

}

void ForwardStep::RenderStep() {

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view_M = camera.GetLookAtMatrix();
	proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	program.Use();
	program.SetMat4("projM", proj_M);
	program.SetMat4("viewM", view_M); 

	for (int i = 0; i < models.size(); i++) {

		LightCube* aux = dynamic_cast<LightCube*>(models.at(i));

		model = glm::mat4(1.0f);
		model = glm::translate(model, models.at(i)->transform.position);
		model = glm::scale(model, models.at(i)->transform.scale);
		program.SetMat4("modelM", model);

		if (aux != nullptr) {
			program.SetVec3("lightColor", aux->color);
		}else {
			program.SetMat4("normalM", glm::transpose(glm::inverse(model)));
		}

		models.at(i)->Draw(program);
	}
}