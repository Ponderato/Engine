#include "GeometryStep.h"

GeometryStep::GeometryStep(Camera* camera, Program& program)
	: camera(camera), program(program) {
}


void GeometryStep::RenderStep() {

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.Use();
	program.SetMat4("projM", camera->GetProjectionMatrix());
	program.SetMat4("viewM", camera->GetLookAtMatrix());

	for (int i = 0; i < models.size(); i++) {

		if (!models.at(i)->hasTex) {
			program.SetInt("hasTex", 0);
		}
		else {
			program.SetInt("hasTex", 1);
		}

		Model* auxModel = dynamic_cast<Model*>(models.at(i));

		program.SetMat4("modelM", models.at(i)->transform.globalModel);
		program.SetMat4("normalM", glm::transpose(glm::inverse(models.at(i)->transform.globalModel)));
		auxModel->Draw(program);
	}
}