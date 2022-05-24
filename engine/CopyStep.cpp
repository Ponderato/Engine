#include "CopyStep.h"

CopyStep::CopyStep(GLbitfield mask, unsigned int* drawBuffer, unsigned int width, unsigned int height, int readAttachment, int drawAttachment) : width(width), height(height){

	this->mask = mask;
	this->drawBuffer = drawBuffer;
	this->readAttachment = readAttachment;
	this->drawAttachment = drawAttachment;
}

//Copy data from FBO to drawBuffer and binds the last one
void CopyStep::RenderStep() {

	glBindFramebuffer(GL_READ_FRAMEBUFFER, *FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *drawBuffer); 
	glReadBuffer(mask + readAttachment);
	glDrawBuffer(mask + drawAttachment);

	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, GL_NEAREST);
}
