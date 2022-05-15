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

//Initialize data
void Context::InitData() {

	camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 0.1f, 45.0f, -90.0f, 0.0f);

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
	//NOW THEN STEPS CONTAIN THE NODES(CUBES,MODELS) THAT ARE GOING TO BE RENDERED. CHANGE THIS SO THE NODES RENDER INDIVIDUALY AND THEY CALL THE STEPS.
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

void Context::InitCube(std::string diffuse, std::string specular, std::string emissive, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* parent) {
	cubes.push_back(new Cube(diffuse, specular, emissive, position, scale, rotation, parent));
}

void Context::InitCube(std::string diffuse, std::string specular, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation) {
	cubes.push_back(new Cube(diffuse, specular, position, scale, rotation));
}

void Context::InitCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation) {
	cubes.push_back(new Cube(position, scale, rotation));
}

void Context::InitLightCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, glm::vec3 color) {
	lightCubes.push_back(new LightCube(position, scale, rotation, color));
}

void Context::InitModel(const std::string& path, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation) {
	models.push_back(new AssimpModel(path, position, scale, rotation));
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


