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

void initOGL() {

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW); //Counter Clock Wise. Specifies the orientation of the fron-facing polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //How polygons will be rasterized. 
	//glEnable(GL_CULL_FACE); //Enable culling for not visible faces. Take into account that in our scene all objects are opaque.
}


