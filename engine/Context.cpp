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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
	cubes.push_back(Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", "../../textures/container2_emissive.jpg", cubePositions[0], glm::vec3(1.0f)));
	cubes.push_back(Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[1], glm::vec3(1.0f)));

	for (int i = 2; i < 7; i++) {
		cubes.push_back(Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[i], glm::vec3(1.0f)));
	}

	for (int i = 0; i < 3; i++) {
		lightCubes.push_back(LightCube(lightPos[i], lightColor[i], glm::vec3(0.25f)));
	}

	camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 0.1f, 45.0f, -90.0f, 0.0f);

	models.push_back(AssimpModel("../../models/shiba/shiba.obj", glm::vec3(2, -2, 0), glm::vec3(100)));

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

	//Steps intialization
	gStep = new GeometryStep(camera, programs[2], cubes, models);
	lStep = new LightingStep(camera, programs[3], gPos, gNorm, gColorSpec);
	cStep = new CopyStep(GL_DEPTH_BUFFER_BIT, WIDTH, HEIGHT);
	fStep = new ForwardStep(camera, programs[1], lightCubes);

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

//Render
void Context::Render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-----------STEPS-------------------
	gStep->RenderStep(gBuffer, gBuffer);
	lStep->RenderStep(defaultFBuffer, defaultFBuffer);
	cStep->RenderStep(gBuffer, defaultFBuffer);
	fStep->RenderStep(defaultFBuffer, defaultFBuffer);
}


