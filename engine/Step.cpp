#include "Step.h"

//Attach the textures and calls de glDrawBuffers function
void Step::AttachTextures() {

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

void Step::SetUp_Buffer(const unsigned int WIDTH, const unsigned int HEIGHT) {

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

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