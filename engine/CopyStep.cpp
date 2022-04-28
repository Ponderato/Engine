#include "CopyStep.h"

//MAKE IT WORK WITH ANY DRAWBUFFER!!!!!!!!!!!!
//Copy data from one framebuffer(read) to another(draw) and binds the last one
void CopyStep::RenderStep(unsigned int* readBuffer, unsigned int* drawBuffer, GLbitfield mask, unsigned int width, unsigned int height) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, *readBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}