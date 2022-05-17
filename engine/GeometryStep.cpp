#include "GeometryStep.h"

GeometryStep::GeometryStep(Camera& camera, Program& program, std::vector<Model*> models)
	: camera(camera), program(program), models(models) {

}

//Attach the textures and calls de glDrawBuffers function
void GeometryStep::AttachTextures() {

	std::vector<unsigned int> attachmentsAux;

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

	for (unsigned int i = 0; i < dataTextures.size(); i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, *dataTextures.at(i), 0);
		attachmentsAux.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	//Convert vector to an array so it can be passed to glDrawBuffers()
	unsigned int* attachments = &attachmentsAux[0];

	//Finally call glDrawBuffers
	glDrawBuffers(attachmentsAux.size(), attachments);
}

void GeometryStep::SetUp_gBuffer(const unsigned int WIDTH, const unsigned int HEIGHT) {

	glGenFramebuffers(1, FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

	//Textures
	for (GLuint* texture : dataTextures) {
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	AttachTextures();

	//Depth buffer
	unsigned int depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	//Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!!!!!" << std::endl;
	}
}

void GeometryStep::RenderStep() {

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view_M = camera.GetLookAtMatrix();
	proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	program.Use();
	program.SetMat4("projM", proj_M);
	program.SetMat4("viewM", view_M);

	for (int i = 0; i < models.size(); i++) {

		//TODO -> SACAR EL UPDATE DE AQUÍ ( METERLO EN EL CONTEXTO), ESTE RENDER SOLO SE TIENE QUE PREOCUPAR DE PASARLE AL SHADER LO QUE HAGA FALTA Y DE 
		//PINTAR EL MODELO
		models.at(i)->Update();

		program.SetMat4("modelM", models.at(i)->transform.globalModel);
		program.SetMat4("normalM", glm::transpose(glm::inverse(models.at(i)->transform.globalModel)));
		models.at(i)->Draw(program);
	}
}