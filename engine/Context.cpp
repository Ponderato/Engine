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

	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE); //This makes the depth buffer (zbuffer) for read only. Do not update the depth buffer.

	glFrontFace(GL_CCW); //Counter Clock Wise. Specifies the orientation of the front-facing polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //How polygons will be rasterized. 
	//glEnable(GL_CULL_FACE); //Enable culling for not visible faces. Take into account that in our scene all objects are opaque.
}

void Context::SetUniforms() {


}

void Context::SetDefaultPipeline() {

	//Uniforms
	programs[3].Use();
	programs[3].SetInt("gPos", 0);
	programs[3].SetInt("gNorm", 1);
	programs[3].SetInt("gColorSpec", 2);

	programs[3].SetMultipleVec3("lightPosition", 3, lightPos);
	programs[3].SetMultipleVec3("lightColor", 3, lightColor);

	//Pipeline configuration
	pipeline = new Pipeline();
	pipeline->SetStep(new GeometryStep(camera, programs[2]));
	pipeline->SetStep(new LightingStep(camera, programs[3]));
	pipeline->SetStep(new CopyStep(GL_DEPTH_BUFFER_BIT, &defaultFBuffer, WIDTH, HEIGHT));
	pipeline->SetStep(new ForwardStep(camera, programs[1]));

	pipeline->gStep->SetFBO(&gBuffer);
	pipeline->gStep->SetInputTexture(0, &gPos);
	pipeline->gStep->SetInputTexture(1, &gNorm);
	pipeline->gStep->SetInputTexture(2, &gColorSpec);
	pipeline->gStep->SetUp_gBuffer(WIDTH, HEIGHT);
	pipeline->gStep->SetProjectionMatrix(projM);

	pipeline->lStep->SetFBO(&defaultFBuffer);
	pipeline->lStep->SetInputTexture(0, pipeline->gStep->GetOutputTexture(0));
	pipeline->lStep->SetInputTexture(1, pipeline->gStep->GetOutputTexture(1));
	pipeline->lStep->SetInputTexture(2, pipeline->gStep->GetOutputTexture(2));

	pipeline->cStep->SetFBO(&gBuffer);

	pipeline->fStep->SetFBO(&defaultFBuffer);
	pipeline->fStep->SetProjectionMatrix(projM);
}

void Context::InitCamera(const glm::vec3 pos, const glm::vec3 worldUp, const float speed, const float sensitivity, const float fov, const float yaw, const float pitch) {
	camera = Camera(pos, worldUp, speed, sensitivity, fov, yaw, pitch);
}

void Context::InitCube(std::string diffuse, std::string specular, std::string emissive, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* parent) {
	models.push_back(new Cube(diffuse, specular, emissive, position, scale, rotation, parent));
}

void Context::InitCube(std::string diffuse, std::string specular, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* parent) {
	models.push_back(new Cube(diffuse, specular, position, scale, rotation, parent));
}

void Context::InitCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* parent) {
	models.push_back(new Cube(position, scale, rotation, parent));
}

void Context::InitLightCube(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, glm::vec3 color, Node* parent) {
	models.push_back(new LightCube(position, scale, rotation, color, parent));
}

void Context::InitModel(const std::string& path, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation, Node* parent) {
	models.push_back(new AssimpModel(path, position, scale, rotation, parent));
}

void Context::InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath) {
	programs.push_back(Program(vertexShaderPath, fragmentShaderPath));
}

void Context::CheckRenderable() {

	renderableModels.clear();
	renderableForwardModels.clear();

	for (Model* model : models) {
		if (model->renderable) {
			if (model->forward) {
				renderableForwardModels.push_back(model);
			}
			else {
				renderableModels.push_back(model);
			}
		}
	}

	pipeline->gStep->SetModels(renderableModels);
	pipeline->fStep->SetModels(renderableForwardModels);
}

void Context::UpdateModels() {
	for (Model* model : models) {
		model->Update();
	}
}

//Render
void Context::Update() {

	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//models[7]->Move(glm::vec3(0.0f, 0.005f, 0.0f), deltaTime);
	models[7]->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 17.0f, 2 *  deltaTime);
	//models[7]->Scale(1.001f);

	UpdateModels();
	CheckRenderable();

	pipeline->Render();
}


