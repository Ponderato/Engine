#include "ForwardStep.h"

ForwardStep::ForwardStep(Camera* camera, Program& program, std::string activePipe) 
	: camera(camera), program(program), activePipe(activePipe){

}

void ForwardStep::RenderStep() {

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

	if (activePipe == "Forward")
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.Use();
	program.SetMat4("projM", camera->GetProjectionMatrix());
	program.SetMat4("viewM", camera->GetLookAtMatrix());

	program.SetVec3("viewerPos", camera->GetPosition());

	if (activePipe == "Forward")
		SetLightUniforms();

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

void ForwardStep::SetLightUniforms() {

	int count = 0;

	for (Node* node : models) {

		LightCube* light = dynamic_cast<LightCube*>(node);

		if (light) {
			program.SetFloat("intensity[" + std::to_string(count) + "]", light->intensity);
			program.SetVec3("lightPosition[" + std::to_string(count) + "]", glm::vec3(light->transform.globalModel[3].x, light->transform.globalModel[3].y, light->transform.globalModel[3].z));
			program.SetVec3("lightColor[" + std::to_string(count) + "]", light->color);
			count += 1;
		}
	}

	program.SetInt("nLights", count);
}
