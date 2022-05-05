#include "CopyStep.h"

CopyStep::CopyStep(GLbitfield mask, unsigned int* drawBuffer, unsigned int width, unsigned int height) : width(width), height(height){

	this->mask = mask;
	this->drawBuffer = drawBuffer;
}

//Copy data from one framebuffer(read) to another(draw) and binds the last one
void CopyStep::RenderStep() {

	glBindFramebuffer(GL_READ_FRAMEBUFFER, *FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *drawBuffer); 
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, *drawBuffer);
}
