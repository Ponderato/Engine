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

	//Objects
	//../ refers to the parent folder, so we need two of them to get to the textures folder
	cubes.push_back(new Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", "../../textures/container2_emissive.jpg", cubePositions[0], glm::vec3(1.0f)));
	cubes.push_back(new Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[1], glm::vec3(1.0f)));

	for (int i = 2; i < 7; i++) {
		cubes.push_back(new Cube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[i], glm::vec3(1.0f)));
	}

	for (int i = 0; i < 3; i++) {
		lightCubes.push_back(new LightCube(lightPos[i], glm::vec3(0.25f), lightColor[i]));
	}

	camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 0.1f, 45.0f, -90.0f, 0.0f);

	models.push_back(new AssimpModel("../../models/shiba/shiba.obj", glm::vec3(2, -2, 0), glm::vec3(100)));

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

	//Pipeline configuration
	pipeline = new Pipeline();
	pipeline->SetStep(new GeometryStep(camera, programs[2], cubes, models));
	pipeline->SetStep(new LightingStep(camera, programs[3]));
	pipeline->SetStep(new CopyStep(GL_DEPTH_BUFFER_BIT, &defaultFBuffer, WIDTH, HEIGHT));
	pipeline->SetStep(new ForwardStep(camera, programs[1], lightCubes));

	pipeline->gStep->SetFBO(&gBuffer);
	pipeline->gStep->SetInputTexture(0, &gPos);
	pipeline->gStep->SetInputTexture(1, &gNorm);
	pipeline->gStep->SetInputTexture(2, &gColorSpec);
	pipeline->gStep->SetUp_gBuffer(WIDTH, HEIGHT);

	pipeline->lStep->SetFBO(&defaultFBuffer);
	pipeline->lStep->SetInputTexture(0, pipeline->gStep->GetOutputTexture(0));
	pipeline->lStep->SetInputTexture(1, pipeline->gStep->GetOutputTexture(1));
	pipeline->lStep->SetInputTexture(2, pipeline->gStep->GetOutputTexture(2));

	pipeline->cStep->SetFBO(&gBuffer);

	pipeline->fStep->SetFBO(&defaultFBuffer);

	//Get max number of color attachments
	//int maxColorAttachments;
	//glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
	//
	//std::cout << maxColorAttachments << std::endl;
}

//Initialize the shaders
void Context::InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath) {
	programs.push_back(Program(vertexShaderPath, fragmentShaderPath));
}

//Render
void Context::Render() {

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-----------STEPS-------------------
	pipeline->Render();
}


