#include "LightingStep.h"

void LightingStep::RenderStep(Camera& camera, Program& program, unsigned int* gPos, unsigned int* gNorm, unsigned int* gColorSpec){

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.Use();
	program.SetVec3("viewerPos", camera.position);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *gNorm);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *gColorSpec);

	RenderQuad();
}

void LightingStep::RenderQuad() {

	if (quadVAO == 0)
	{
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
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}