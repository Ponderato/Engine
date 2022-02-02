#include <gl/glew.h>
#include <iostream>
#include <stdio.h>
#include "oglContext.h"


void initGLEW() {
	//We initialize glew. GLEW sets the pointer functions for your platform.
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("Error: %s\n", glewGetErrorString(err));
		exit(-1);
	}
}

