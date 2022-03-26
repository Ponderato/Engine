#include "oglContext.h"

glm::vec3 lightPos[] = {
	glm::vec3(2.0f, 2.0f, 0.0f),
	glm::vec3(-2.0f, -3.0f, -4.0f),
	glm::vec3(1.5f, 1.0f, -6.0f)
};
glm::vec3 lightColor[] = {
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
};

glm::vec3 cubePositions[] = {
	glm::vec3(-1.0f,  1.0f,  -1.5f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-2.5f, -4.2f, -4.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -1.4f, -3.5f)
};

glm::mat4 view_M;
glm::mat4 proj_M = glm::mat4(1.0f);
glm::mat4 normal_M;

//Initialize GLEW library.
void initGLEW() {

	//We initialize glew. GLEW sets the pointer functions for your platform.
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("Error: %s\n", glewGetErrorString(err));
		exit(-1);
	}
}

//Initialize OpenGL.
void initOGL() {

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW); //Counter Clock Wise. Specifies the orientation of the front-facing polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //How polygons will be rasterized. 
	//glEnable(GL_CULL_FACE); //Enable culling for not visible faces. Take into account that in our scene all objects are opaque.
}

//Initialize data
void initData() {

	//Objects
	cubes[0] = Cube("container2.jpg", "container2_specular.jpg", "container2_emissive.jpg");
	cubes[1] = Cube("container2.jpg", "container2_specular.jpg");

	for (int i = 2; i < CUBES_COUNT; i++) {
		cubes[i] = Cube("container2.jpg", "container2_specular.jpg");
	}

	camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 0.1f, 45.0f, -90.0f, 0.0f);

	//Uniforms
	programs[0].Use();
	programs[0].SetMultipleVec3("lightPosition", 3, lightPos);
	programs[0].SetMultipleVec3("lightColor", 3, lightColor);
}

//Initialize the shaders
void initShaders(int shaderNum, const char* vertexShaderPath, const char* fragmentShaderPath) {
	programs[shaderNum] = Program(vertexShaderPath, fragmentShaderPath);
}

//Render stuff
void render() {

	static float angle = 0.0f;
	angle = (angle > 360) ? 0 : angle + 0.01f;

	//view_M = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
	view_M = camera->GetLookAtMatrix();

	proj_M = glm::perspective(glm::radians(camera->fov), 800.0f / 600.0f, 0.1f, 100.0f);

	glm::mat4 model_M = glm::mat4(1.0f);
	model_M = glm::rotate(model_M, (float)angle * glm::radians(50.0f), glm::vec3(0.5, 1.0, 0.0));

	glm::mat4 model_M_2 = glm::mat4(1.0f);
	model_M_2 = glm::translate(model_M_2, glm::vec3(-3.0, 0.0, 0.0));
	model_M_2 = glm::rotate(model_M_2, (float)angle * glm::radians(-45.0f) * 3, glm::vec3(0.0, 1.0, 0.0));
	model_M_2 = glm::rotate(model_M_2, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

	static bool c = false;

	static float t = 0.0F;
	t = (t < 1) ? t + 0.001f : 0.0f;

	//First curve
	if (c == false) {
		model_M_2[3].x = 4.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * 4.0f * t * (1.0f - t) * (1.0f - t) + 3.0f * (-4.0f) * t * t * (1.0f - t) + (-4.0f) * t * t * t;
		model_M_2[3].z = 0.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * (-6.0f) * t * (1.0f - t) * (1.0f - t) + 3.0f * (-6.0f) * t * t * (1.0f - t) + 0.0f * t * t * t;
		if (t >= 1) {
			t = 0.0f;
			c = true;
		}
	}
	//Second one
	if (c == true) {
		model_M_2[3].x = (-4.0f) * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * (-4.0f) * t * (1.0f - t) * (1.0f - t) + 3.0f * 4.0f * t * t * (1.0f - t) + 4.0f * t * t * t;
		model_M_2[3].z = 0.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * 6.0f * t * (1.0f - t) * (1.0f - t) + 3.0f * 6.0f * t * t * (1.0f - t) + 0.0f * t * t * t;
		if (t >= 1) {
			c = false;
		}
	}

	//Not necessary to do it in the render func. Only needs to be done once
	//Matrices calculations for the ligh cubes
	glm::mat4 model_M_3 = glm::mat4(1.0f);
	model_M_3 = glm::translate(model_M_3, lightPos[0]);
	model_M_3 = glm::scale(model_M_3, glm::vec3(0.25f));

	glm::mat4 model_M_4 = glm::mat4(1.0f);
	model_M_4 = glm::translate(model_M_4, lightPos[1]);
	model_M_4 = glm::scale(model_M_4, glm::vec3(0.25f));

	glm::mat4 model_M_5 = glm::mat4(1.0f);
	model_M_5 = glm::translate(model_M_5, lightPos[2]);
	model_M_5 = glm::scale(model_M_5, glm::vec3(0.25f));
	//--------------------------------------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	programs[0].Use();

	programs[0].SetVec3("spotlightDir", camera->front);
	programs[0].SetVec3("spotlightPos", camera->position);

	//glUniform1f(glGetUniformLocation(shaderProgram[0], "faceV"), faceVisibility);

	programs[0].SetMat4("viewM", view_M);
	programs[0].SetMat4("projM", proj_M);

	programs[0].SetVec3("viewerPos", camera->position);

	//Central Cube
	normal_M = glm::transpose(glm::inverse(model_M));
	programs[0].SetMat4("normalM", normal_M);
	programs[0].SetMat4("modelM", model_M);
	cubes[0].Draw(programs[0]);

	//Orbital cube
	normal_M = glm::transpose(glm::inverse(model_M_2));
	programs[0].SetMat4("normalM", normal_M);
	programs[0].SetMat4("modelM", model_M_2);
	cubes[1].Draw(programs[0]);

	//Extra cubes
	for (int i = 0; i < 5; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f + i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		normal_M = glm::transpose(glm::inverse(model));
		programs[0].SetMat4("normalM", normal_M);
		programs[0].SetMat4("modelM", model);
		cubes[i + 2].Draw(programs[0]);
	}

	//Lightcubes
	programs[1].Use();

	programs[1].SetMat4("viewM", view_M);
	programs[1].SetMat4("projM", proj_M);

	programs[1].SetMat4("modelM", model_M_3);
	programs[1].SetVec3("lightColor", lightColor[0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	programs[1].SetMat4("modelM", model_M_4);
	programs[1].SetVec3("lightColor", lightColor[1]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	programs[1].SetMat4("modelM", model_M_5);
	programs[1].SetVec3("lightColor", lightColor[2]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

}

