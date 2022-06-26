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

void Context::SetPipeline() {

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

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Pipeline configuration
	pipeline = new Pipeline();
	pipeline->SetStep(new GeometryStep(camera, programs[2]));
	pipeline->SetStep(new LightingStep(camera, programs[3]));
	pipeline->SetStep(new CopyStep(GL_DEPTH_BUFFER_BIT, &middleBuffer, WIDTH, HEIGHT, 0, 0));
	pipeline->SetStep(new ForwardStep(camera, programs[1]));

	pipeline->gStep->SetFBO(&gBuffer);
	pipeline->gStep->SetInputTexture(0, &gPos);
	pipeline->gStep->SetInputTexture(1, &gNorm);
	pipeline->gStep->SetInputTexture(2, &gColorSpec);
	pipeline->gStep->SetUp_Buffer(WIDTH, HEIGHT);

	pipeline->lStep->SetFBO(&middleBuffer);
	pipeline->lStep->SetInputTexture(0, pipeline->gStep->GetOutputTexture(0));
	pipeline->lStep->SetInputTexture(1, pipeline->gStep->GetOutputTexture(1));
	pipeline->lStep->SetInputTexture(2, pipeline->gStep->GetOutputTexture(2));
	pipeline->lStep->SetInputTexture(3, &renderTexture);
	pipeline->lStep->SetUp_Buffer(WIDTH, HEIGHT);

	pipeline->cStep->SetFBO(&gBuffer);

	pipeline->fStep->SetFBO(&middleBuffer);

	//Finally set the uniforms
	SetDSUniforms();
}

void Context::InitCamera(const glm::vec3 pos, const glm::vec3 worldUp, const float speed, const float sensitivity, const float fov, const float yaw, const float pitch, Node* parent) {
	camera = new Camera(pos, worldUp, speed, sensitivity, fov, yaw, pitch, parent);
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

	pipeline->gStep->SetModels(renderableModels);
	pipeline->fStep->SetModels(renderableForwardModels);
	pipeline->lStep->SetModels(nodes);
}

void Context::UpdateModels() {
	for (Node* model : nodes) {
		model->Update();
	}
}

void Context::UpdateCamera() {
	//for (Node* node : nodes) {
	//	Camera* camera = dynamic_cast<Camera*>(node);
	//	if (camera)
	//		camera->UpdateL();
	//		camera->UpdatePM();
	//}
	camera->UpdatePM();
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

	UpdateCamera();

	SetDSUniforms();

	pipeline->Render();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


