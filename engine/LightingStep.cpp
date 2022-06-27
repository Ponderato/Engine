#include "LightingStep.h"

LightingStep::LightingStep(Camera* camera, Program& program)
	: camera(camera), program(program) {

	//Setup the plane VAO
	//The location numbers have to match with the lightingPass_vs layout ins
	//locations, since that is the shader that will be active when rendering
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void LightingStep::RenderStep(){

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

	program.Use();
	program.SetVec3("viewerPos", camera->GetPosition());
	SetLightUniforms();

	for (unsigned int i = 0; i < dataTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, *dataTextures.at(i));
	}
	
	RenderQuad();
}

void LightingStep::RenderQuad() {

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void LightingStep::SetLightUniforms() {

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