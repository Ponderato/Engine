#include "CopyStep.h"

//Copy data from one framebuffer(read) to another(draw) and binds the last one
void CopyStep::RenderStep(unsigned int* readBuffer, unsigned int* drawBuffer, GLbitfield mask, unsigned int width, unsigned int height) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, *readBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *drawBuffer); 
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, *drawBuffer);
}

//Copy data from one framebuffer(read) to the default framebuffer and binds it
void CopyStep::RenderStep(unsigned int* readBuffer, GLbitfield mask, unsigned int width, unsigned int height) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, *readBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}