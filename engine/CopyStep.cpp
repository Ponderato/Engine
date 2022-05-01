#include "CopyStep.h"

CopyStep::CopyStep(GLbitfield mask, unsigned int width, unsigned int height) : width(width), height(height){

	this->mask = mask;
}

//Copy data from one framebuffer(read) to another(draw) and binds the last one
void CopyStep::RenderStep(unsigned int& readBuffer, unsigned int& drawBuffer) {

	glBindFramebuffer(GL_READ_FRAMEBUFFER, readBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawBuffer); 
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, drawBuffer);
}
