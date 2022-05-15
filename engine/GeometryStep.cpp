#include "GeometryStep.h"

GeometryStep::GeometryStep(Camera& camera, Program& program, std::vector<Model*> cubes, std::vector<Model*> models)
	: camera(camera), program(program), cubes(cubes), models(models) {

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
	for each (GLuint* texture in dataTextures) {
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GeometryStep::RenderStep() {

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view_M = camera.GetLookAtMatrix();
	proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	program.Use();
	program.SetMat4("projM", proj_M);
	program.SetMat4("viewM", view_M);

	//MAYBE GEOMETRYDATA INSTANCE SO NO NEED OF TWO FOR LOOPS. JUST ONE ITERATING OVER THE 
	//DATA OF GEOMETRYDATA

	for (int i = 0; i < cubes.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubes.at(i)->transform.position);	
		model = glm::rotate(model, glm::radians(cubes.at(i)->transform.rotation.w), glm::vec3(cubes.at(i)->transform.rotation.x, cubes.at(i)->transform.rotation.y, cubes.at(i)->transform.rotation.z));
		model = glm::scale(model, cubes.at(i)->transform.scale);
		normal = glm::transpose(inverse(model));

		//Update
		cubes.at(i)->Update();

		program.SetMat4("modelM", model);
		program.SetMat4("normalM", normal);
		cubes.at(i)->Draw(program);
	}

	for (int i = 0; i < models.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, models.at(i)->transform.position);
		model = glm::rotate(model, glm::radians(models.at(i)->transform.rotation.w), glm::vec3(models.at(i)->transform.rotation.x, models.at(i)->transform.rotation.y, models.at(i)->transform.rotation.z));
		model = glm::scale(model, models.at(i)->transform.scale);
		normal = glm::transpose(glm::inverse(model));

		models.at(i)->Update();

		program.SetMat4("modelM", model);
		program.SetMat4("normalM", normal);
		models.at(i)->Draw(program);
	}
}