#include "Context.h"

//Empty constructor
Context::Context() {

}

//Initialize GLEW library.
void Context::InitGLEW() {

	//We initialize glew. GLEW sets the pointer functions for your platform.
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("Error: %s\n", glewGetErrorString(err));
		exit(-1);
	}
}

//Initialize OpenGL.
void Context::InitOGL() {

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	//glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE); //This makes the depth buffer (zbuffer) for read only. Do not update the depth buffer.

	glFrontFace(GL_CCW); //Counter Clock Wise. Specifies the orientation of the front-facing polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //How polygons will be rasterized. 
	//glEnable(GL_CULL_FACE); //Enable culling for not visible faces. Take into account that in our scene all objects are opaque.
}

//!!!!!!!!!!!!!!!!!!!!SEPARATE THIS METHOD into initCubeData() in order to make 
//!!!!!!!!!!!!!!!! possible to render different cubes with different textures
//Initialize data
void Context::InitData() {

	ConfigureG_Buffer();

	//Objects
	//../ refers to the parent folder, so we need two of them to get to the textures folder
	cubes.push_back(Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", "../../textures/container2_emissive.jpg"));
	cubes.push_back(Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg"));

	for (int i = 2; i < 7; i++) {
		cubes.push_back(Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg"));
	}

	for (int i = 0; i < 3; i++) {
		lightCubes.push_back(Cube());
	}

	camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 0.1f, 45.0f, -90.0f, 0.0f);

	models.push_back(Model("../../models/shiba/shiba.obj"));

	//------------------UNIFORMS (lighting info)-----------------
	//programs[0].Use();
	//programs[0].SetMultipleVec3("lightPosition", 3, lightPos);
	//programs[0].SetMultipleVec3("lightColor", 3, lightColor);

	//Deferred Shading
	programs[3].Use();
	programs[3].SetInt("gPos", 0);
	programs[3].SetInt("gNorm", 1);
	programs[3].SetInt("gColorSpec", 2);
			 
	programs[3].SetMultipleVec3("lightPosition", 3, lightPos);
	programs[3].SetMultipleVec3("lightColor", 3, lightColor);
}

//Initialize the shaders
void Context::InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath) {
	programs.push_back(Program(vertexShaderPath, fragmentShaderPath));
}

//Configure g_buffer framebuffer for deferred shading
void Context::ConfigureG_Buffer() {

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//Position color buffer
	glGenTextures(1, &gPos);
	glBindTexture(GL_TEXTURE_2D, gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPos, 0);

	//Normal color buffer
	glGenTextures(1, &gNorm);
	glBindTexture(GL_TEXTURE_2D, gNorm);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNorm, 0);

	//Color + Specular color buffer
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

	//Tell OpenGL which color attachments we are rendering into.
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

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

//Render the deferred shading QUAD
void Context::RenderQuad() {
	if (quadVAO == 0)
	{
		//Setup the plane VAO
		//The location numbers have to match with the lightingPass_vs layout ins
		//locations, since that is the shader that will be active when rendering

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


//Render
void Context::Render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//-----------STEPS-------------------
	//MAKE IT MOREEEEEEE GENERIC. IT HAS TO WORK WITH ANYTHING. THE PARAMETERS BELOW  NOT VERY EFFECTIVE

	gStep.RenderStep(camera, programs[2], &gBuffer, &cubes, cubePositions, &models);
	lStep.RenderStep(camera, programs[3], &gPos, &gNorm, &gColorSpec);
	cStep.RenderStep(&gBuffer, 0, GL_DEPTH_BUFFER_BIT, WIDTH, HEIGHT);
	fStep.RenderStep(camera, programs[1], &lightCubes, lightPos, lightColor);


	/*

	view_M = camera.GetLookAtMatrix();
	proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	//------------------MATRICES CALCULATIONS-------------------------------
	//
	////angle, c and t need to be static in order to work. When render() is
	////called from main, these variables are generated and held in memory when
	////render has finished.
	////If they were non static, they would not be held and the cubes would not 
	////move.
	//static float angle = 0.0f;
	//angle = (angle > 360) ? 0 : angle + 0.01f;
	//
	////view_M = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
	//view_M = camera.GetLookAtMatrix();
	//
	//proj_M = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);
	//
	//glm::mat4 model_M = glm::mat4(1.0f);
	//model_M = glm::rotate(model_M, (float)angle * glm::radians(50.0f), glm::vec3(0.5, 1.0, 0.0));
	//
	//glm::mat4 model_M_2 = glm::mat4(1.0f);
	//model_M_2 = glm::translate(model_M_2, glm::vec3(-3.0, 0.0, 0.0));
	//model_M_2 = glm::rotate(model_M_2, (float)angle * glm::radians(-45.0f) * 3, glm::vec3(0.0, 1.0, 0.0));
	//model_M_2 = glm::rotate(model_M_2, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	//
	//static bool c = false;
	//
	//static float t = 0.0F;
	//t = (t < 1) ? t + 0.001f : 0.0f;
	//
	////First curve
	//if (c == false) {
	//	model_M_2[3].x = 4.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * 4.0f * t * (1.0f - t) * (1.0f - t) + 3.0f * (-4.0f) * t * t * (1.0f - t) + (-4.0f) * t * t * t;
	//	model_M_2[3].z = 0.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * (-6.0f) * t * (1.0f - t) * (1.0f - t) + 3.0f * (-6.0f) * t * t * (1.0f - t) + 0.0f * t * t * t;
	//	if (t >= 1) {
	//		t = 0.0f;
	//		c = true;
	//	}
	//}
	////Second curve
	//if (c == true) {
	//	model_M_2[3].x = (-4.0f) * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * (-4.0f) * t * (1.0f - t) * (1.0f - t) + 3.0f * 4.0f * t * t * (1.0f - t) + 4.0f * t * t * t;
	//	model_M_2[3].z = 0.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * 6.0f * t * (1.0f - t) * (1.0f - t) + 3.0f * 6.0f * t * t * (1.0f - t) + 0.0f * t * t * t;
	//	if (t >= 1) {
	//		c = false;
	//	}
	//}
	//
	////Matrices calculations for the ligh cubes
	//glm::mat4 model_M_3 = glm::mat4(1.0f);
	//model_M_3 = glm::translate(model_M_3, lightPos[0]);
	//model_M_3 = glm::scale(model_M_3, glm::vec3(0.25f));
	//
	//glm::mat4 model_M_4 = glm::mat4(1.0f);
	//model_M_4 = glm::translate(model_M_4, lightPos[1]);
	//model_M_4 = glm::scale(model_M_4, glm::vec3(0.25f));
	//
	//glm::mat4 model_M_5 = glm::mat4(1.0f);
	//model_M_5 = glm::translate(model_M_5, lightPos[2]);
	//model_M_5 = glm::scale(model_M_5, glm::vec3(0.25f));

	//---------------------------------------------------------------------

	//-------------------------GEOMETRY PASS-------------------------------
	//Render scene´s geometry/color data into gBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	programs[2].Use();
	programs[2].SetMat4("projM", proj_M);
	programs[2].SetMat4("viewM", view_M);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 normal = glm::mat4(1.0f);

	for (int i = 0; i < cubes.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		normal = glm::transpose(inverse(model));
		programs[2].SetMat4("modelM", model);
		programs[2].SetMat4("normalM", normal);
		cubes[i].Draw(programs[2]);
	}

	for (int i = 0; i < models.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f));
		normal = glm::transpose(glm::inverse(model));
		programs[2].SetMat4("modelM", model);
		programs[2].SetMat4("normalM", normal);
		models[i].Draw(programs[2]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//------------------------LIGHTING PASS--------------------------------
	//Calculate lighting using gBuffer's content
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	programs[3].Use();
	programs[3].SetVec3("viewerPos", camera.position);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNorm);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);

	RenderQuad();
	//--------------------------------------------------------------------

	//Copy data from geometry's depth buffer to default framebuffer's one.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); //Write to default framebuffer
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Render the lights with FORWARD RENDERING
	programs[1].Use();
	programs[1].SetMat4("projM", proj_M);
	programs[1].SetMat4("viewM", view_M);
	for (int i = 0; i < lightCubes.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos[i]);
		model = glm::scale(model,glm::vec3(0.25f));
		programs[1].SetMat4("modelM", model);
		programs[1].SetVec3("lightColor", lightColor[i]);
		lightCubes[i].Draw(programs[1]);
	}
	
	//programs[0].Use();
	//
	//programs[0].SetVec3("spotlightDir", camera.front);
	//programs[0].SetVec3("spotlightPos", camera.position);
	//
	////glUniform1f(glGetUniformLocation(shaderProgram[0], "faceV"), faceVisibility);
	//
	//programs[0].SetMat4("viewM", view_M);
	//programs[0].SetMat4("projM", proj_M);
	//
	//programs[0].SetVec3("viewerPos", camera.position);
	//
	//
	////Central Cube
	//normal_M = glm::transpose(glm::inverse(model_M));
	//programs[0].SetMat4("normalM", normal_M);
	//programs[0].SetMat4("modelM", model_M);
	//cubes[0].Draw(programs[0]);
	//
	////Orbital cube
	//normal_M = glm::transpose(glm::inverse(model_M_2));
	//programs[0].SetMat4("normalM", normal_M);
	//programs[0].SetMat4("modelM", model_M_2);
	//cubes[1].Draw(programs[0]);
	//
	////Extra cubes
	//for (int i = 0; i < 5; i++) {
	//	glm::mat4 model = glm::mat4(1.0f);
	//	model = glm::translate(model, cubePositions[i]);
	//	float angle = 20.0f + i;
	//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	//	normal_M = glm::transpose(glm::inverse(model));
	//	programs[0].SetMat4("normalM", normal_M);
	//	programs[0].SetMat4("modelM", model);
	//	cubes[i + 2].Draw(programs[0]);
	//}
	//
	////Model
	//glm::mat4 model_M_6 = glm::mat4(1.0f);
	//model_M_6 = glm::translate(model_M_6, glm::vec3(2.0f, -2.0f, 0.0f));
	//model_M_6 = glm::scale(model_M_6, glm::vec3(100.0f));
	//normal_M = glm::transpose(glm::inverse(model_M_6));
	//programs[0].SetMat4("normalM", normal_M);
	//programs[0].SetMat4("modelM", model_M_6);
	//models[0].Draw(programs[0]);
	//
	////Lightcubes
	//programs[1].Use();
	//
	//programs[1].SetMat4("viewM", view_M);
	//programs[1].SetMat4("projM", proj_M);
	//
	//programs[1].SetMat4("modelM", model_M_3);
	//programs[1].SetVec3("lightColor", lightColor[0]);
	//lightCubes[0].Draw(programs[1]);
	//
	//programs[1].SetMat4("modelM", model_M_4);
	//programs[1].SetVec3("lightColor", lightColor[1]);
	//lightCubes[1].Draw(programs[1]);
	//
	//programs[1].SetMat4("modelM", model_M_5);
	//programs[1].SetVec3("lightColor", lightColor[2]);
	//lightCubes[2].Draw(programs[1]);

	
	*/
}


