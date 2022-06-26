#include "ForwardStep.h"

ForwardStep::ForwardStep(Camera* camera, Program& program) 
	: camera(camera), program(program){

}

void ForwardStep::RenderStep() {

	//glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

	program.Use();
	program.SetMat4("projM", camera->GetProjectionMatrix());
	program.SetMat4("viewM", camera->GetLookAtMatrix());

	for (unsigned int i = 0; i < dataTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, *dataTextures.at(i));
	}

	for (int i = 0; i < models.size(); i++) {

		LightCube* aux = dynamic_cast<LightCube*>(models.at(i));
		Model* auxModel = dynamic_cast<Model*>(models.at(i));

		program.SetMat4("modelM", models.at(i)->transform.globalModel);

		if (aux != nullptr) {
			program.SetVec3("color", aux->color);
		}else {
			program.SetMat4("normalM", glm::transpose(glm::inverse(models.at(i)->transform.globalModel)));
		}

		auxModel->Draw(program);
		
	}
}

