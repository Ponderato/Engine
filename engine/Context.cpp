#include "Context.h"

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

//Set deferred shading uniforms
void Context::SetDSUniforms() {

	//Deferred Shading
	programs[3].Use();
	programs[3].SetInt("gPos", 0);
	programs[3].SetInt("gNorm", 1);
	programs[3].SetInt("gColorSpec", 2);
}

void Context::SetDeferredPipeline() {

	//First we create the buffers and get they'r Id's
	glGenFramebuffers(1, &gBuffer);
	glGenFramebuffers(1, &middleBuffer);

	GetFrameBufferID(&gBuffer);
	GetFrameBufferID(&middleBuffer);
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)&defaultBuffer); //It is already binded at the end of previous method, so no need of calling the method again

	//Now we set up the textures
	glGenTextures(1, &gPos);
	glBindTexture(GL_TEXTURE_2D, gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &gNorm);
	glBindTexture(GL_TEXTURE_2D, gNorm);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &renderTextureD);
	glBindTexture(GL_TEXTURE_2D, renderTextureD);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Pipeline configuration
	pipelines.push_back(new Pipeline("Deferred"));
	pipelines.at(pipelines.size() - 1)->SetStep(new GeometryStep(camera, programs[2]));
	pipelines.at(pipelines.size() - 1)->SetStep(new LightingStep(camera, programs[3]));
	pipelines.at(pipelines.size() - 1)->SetStep(new CopyStep(GL_DEPTH_BUFFER_BIT, &middleBuffer, WIDTH, HEIGHT, 0, 0));
	pipelines.at(pipelines.size() - 1)->SetStep(new ForwardStep(camera, programs[1], "Deferred"));
	pipelines.at(pipelines.size() - 1)->SetStep(new FrustumStep(camera, programs[4]));

	pipelines.at(pipelines.size() - 1)->gStep->SetFBO(&gBuffer);
	pipelines.at(pipelines.size() - 1)->gStep->SetInputTexture(0, &gPos);
	pipelines.at(pipelines.size() - 1)->gStep->SetInputTexture(1, &gNorm);
	pipelines.at(pipelines.size() - 1)->gStep->SetInputTexture(2, &gColorSpec);
	pipelines.at(pipelines.size() - 1)->gStep->SetUp_Buffer(WIDTH, HEIGHT);

	pipelines.at(pipelines.size() - 1)->lStep->SetFBO(&middleBuffer);
	pipelines.at(pipelines.size() - 1)->lStep->SetInputTexture(0, pipelines.at(pipelines.size() - 1)->gStep->GetOutputTexture(0));
	pipelines.at(pipelines.size() - 1)->lStep->SetInputTexture(1, pipelines.at(pipelines.size() - 1)->gStep->GetOutputTexture(1));
	pipelines.at(pipelines.size() - 1)->lStep->SetInputTexture(2, pipelines.at(pipelines.size() - 1)->gStep->GetOutputTexture(2));
	pipelines.at(pipelines.size() - 1)->lStep->SetInputTexture(3, &renderTextureD);
	pipelines.at(pipelines.size() - 1)->lStep->SetUp_Buffer(WIDTH, HEIGHT);

	pipelines.at(pipelines.size() - 1)->cStep->SetFBO(&gBuffer);

	pipelines.at(pipelines.size() - 1)->fStep->SetFBO(&middleBuffer);
	pipelines.at(pipelines.size() - 1)->fStep->SetInputTexture(0, &renderTextureD);
	//pipelines.at(pipelines.size() - 1)->fStep->SetUp_Buffer(WIDTH, HEIGHT);

	pipelines.at(pipelines.size() - 1)->frusStep->SetInactiveCameras(this->inactiveCameras);

	//Finally set the uniforms
	SetDSUniforms();

	SetActivePipeline(pipelines.at(pipelines.size() - 1));
}

void Context::SetForwardPipeline() {

	glGenFramebuffers(1, &fbuffer);

	GetFrameBufferID(&fbuffer);

	glGenTextures(1, &renderTextureF);
	glBindTexture(GL_TEXTURE_2D, renderTextureF);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	pipelines.push_back(new Pipeline("Forward"));
	pipelines.at(pipelines.size() - 1)->SetStep(new ForwardStep(camera, programs[0], "Forward"));
	pipelines.at(pipelines.size() - 1)->SetStep(new FrustumStep(camera, programs[4]));

	pipelines.at(pipelines.size() - 1)->fStep->SetFBO(&fbuffer);
	pipelines.at(pipelines.size() - 1)->fStep->SetInputTexture(0, &renderTextureF);
	pipelines.at(pipelines.size() - 1)->fStep->SetUp_Buffer(WIDTH, HEIGHT);

	pipelines.at(pipelines.size() - 1)->frusStep->SetInactiveCameras(this->inactiveCameras);

	SetActivePipeline(pipelines.at(pipelines.size() - 1));
}

void Context::InitCamera(const glm::vec3 pos, const glm::vec3 worldUp, const float aspect, const float speed, const float sensitivity, const float fov, const float yaw, const float pitch, Node* parent) {
	
	Camera* cam = new Camera(pos, worldUp, aspect, speed, sensitivity, fov, yaw, pitch, parent);
	nodes.push_back(cam);
	inactiveCameras.push_back(cam);
	
}

void Context::InitCameraSet(const glm::vec3 pos, const glm::vec3 worldUp, const float aspect, const float speed, const float sensitivity, const float fov, const float yaw, const float pitch, Node* parent) {

	camera = new Camera(pos, worldUp, aspect, speed, sensitivity, fov, yaw, pitch, parent);
	nodes.push_back(camera);
}

void Context::InitCube(std::string diffuse, std::string specular, std::string emissive, glm::vec3 position, Node* parent) {
	nodes.push_back(new Cube(diffuse, specular, emissive, position, parent));
}

void Context::InitCube(std::string diffuse, std::string specular, glm::vec3 position, Node* parent) {
	nodes.push_back(new Cube(diffuse, specular, position, parent));
}

void Context::InitCube(glm::vec3 position, Node* parent) {
	nodes.push_back(new Cube(position,  parent));
}

void Context::InitLightCube(glm::vec3 position, glm::vec3 color, Node* parent) {
	nodes.push_back(new LightCube(position, color, parent));
}

void Context::InitModel(const std::string& path, glm::vec3 position, Node* parent) {
	nodes.push_back(new AssimpModel(path, position, parent));
}

void Context::InitShaders(const char* vertexShaderPath, const char* fragmentShaderPath) {
	programs.push_back(Program(vertexShaderPath, fragmentShaderPath));
}

void Context::DeleteNode(Node* node) {

	if (nodes.size() > 1) {
		int index = 0;

		for (Node* aux : nodes) {
			if (aux->ID == node->ID) {
				if (aux->children.size() > 0) {
					DeleteChildNodes(aux);
				}
				nodes.erase(nodes.begin() + index);
			}
			index++;
		}
		node->parent->children.clear();
	}
}

void Context::DeleteChildNodes(Node* node) {

	for (Node* aux : node->children) {
		if (aux->children.size() > 0) {
			DeleteChildNodes(aux);
		}

		node->children.erase(node->children.begin());

		int index = 0;
		for (Node* aux2 : nodes) {
			if (aux->ID == aux2->ID) {
				nodes.erase(nodes.begin() + index);
			}
			index++;
		}
	}
}

void Context::CheckRenderable() {

	renderableModels.clear();
	renderableForwardModels.clear();

	if (activePipe->Name == "Deferred") {
		for (Node* model : nodes) {
			if (model->renderable) {
				if (model->forward) {
					renderableForwardModels.push_back(model);
				}
				else {
					renderableModels.push_back(model);
				}
			}
		}

		activePipe->gStep->SetModels(renderableModels);
		activePipe->fStep->SetModels(renderableForwardModels);
		activePipe->lStep->SetModels(nodes);
	}
	else {
		for (Node* model : nodes) {
			if (model->renderable) {
				renderableForwardModels.push_back(model);
			}
		}
		activePipe->fStep->SetModels(renderableForwardModels);
	}
}

void Context::UpdateModels() {
	for (Node* model : nodes) {
		model->Update();
	}
}

void Context::UpdateCameras() {
	for (Node* node : nodes) {
		Camera* cam = dynamic_cast<Camera*>(node);
		if (cam) {
			cam->UpdatePM();
		}
	}
}

void Context::SetActiveCamera(Camera* camera) {

	this->camera = camera;

	inactiveCameras.clear();
	for (Node* node : nodes) {
		Camera* cam = dynamic_cast<Camera*>(node);
		if (cam && cam->ID != camera->ID) {
			inactiveCameras.push_back(cam);
		}
	}

	if (activePipe->Name == "Deferred") {
		activePipe->gStep->SetCamera(this->camera);
		activePipe->lStep->SetCamera(this->camera);
	}
	activePipe->fStep->SetCamera(this->camera);
	activePipe->frusStep->SetCamera(this->camera);
	activePipe->frusStep->SetInactiveCameras(this->inactiveCameras);
}

void Context::SetActivePipeline(Pipeline* pipe) {
	
	this->activePipe = pipe;
}

void Context::GetFrameBufferID(unsigned int *framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)framebuffer);

	//Unbind it
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Render
void Context::Update() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UpdateModels();
	CheckRenderable();

	UpdateCameras();

	if (activePipe->Name == "Deferred")
		SetDSUniforms();

	activePipe->Render();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


