#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <glfw3.h>

#include "Context.h"
#include "HierarchyPanel.h"
#include "RenderPanel.h"
#include "InspectorPanel.h"

//Data
const unsigned int OGL_WIDTH = 1000;
const unsigned int OGL_HEIGHT = 600;

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

static bool moveMouse = false;
bool firstMouse = true;
const float sensitivity = 0.1f;
float lastX = OGL_WIDTH / 2;
float lastY = OGL_HEIGHT / 2;

const unsigned int N_LIGHTS = 3;

float deltaTime = 0.0f;//Time between current frame and last frame
float lastFrame = 0.0f;//Time of last frame

//Light & cube data
glm::vec3 lightPos[N_LIGHTS] = {
glm::vec3(2.0f, 2.0f, 0.0f),
glm::vec3(-2.0f, -3.0f, -4.0f),
glm::vec3(1.5f, 1.0f, -6.0f)
};
glm::vec3 lightColor[N_LIGHTS] = {
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
};
glm::vec3 cubePositions[7] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(1.5f,  1.0f,  0.0f),
	glm::vec3(-1.0f,  1.0f,  -1.5f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-2.5f, -4.2f, -4.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -1.4f, -3.5f)
};

Context context;
HierarchyPanel h_panel;
RenderPanel r_panel;
InspectorPanel i_panel;

//Declaration of methods -> C programming stuff :D
GLFWwindow* InitContext();
void InitImGui(GLFWwindow* window);
void InitImGuiFrame();
void FinishImGui();
void RenderImGui();
void SetImGuiWindows();

void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
void Mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void Scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* window);

int main(){

	//Initialization
	GLFWwindow* window = InitContext();

	context.InitOGL();
	InitImGui(window);

	context.InitShaders("default_vs.glsl", "default_fs.glsl");            //programs[0]
	context.InitShaders("lightBox_vs.glsl", "lightBox_fs.glsl");		  //programs[1]
	context.InitShaders("geometryPass_vs.glsl", "geometryPass_fs.glsl");  //programs[2]
	context.InitShaders("lightingPass_vs.glsl", "lightingPass_fs.glsl");  //programs[3]

	#pragma region INITGEO
	//../ refers to the parent folder, so we need two of them to get to the textures folder
	context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", "../../textures/container2_emissive.jpg", cubePositions[0], context.parentNode);
	context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[1], context.nodes[0]);
	for (int i = 2; i < 7; i++) {
		context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[i], context.parentNode);
	}

	context.InitModel("../../models/shiba/shiba.obj", glm::vec3(2, -2, 0), context.parentNode);

	for (int i = 0; i < 3; i++) {
		context.InitLightCube(lightPos[i], lightColor[i], context.parentNode);
	}
	#pragma endregion

	context.InitCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 0.1f, 45.0f, -90.0f, 0.0f, context.parentNode);
	context.camera.SetAspectRatio(WIDTH / HEIGHT);
	
	context.SetWIdth(WIDTH);
	context.SetHeight(HEIGHT);
	context.SetPipeline();
	
	glViewport(0, 0, WIDTH, HEIGHT);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
	
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		

		//Input
		ProcessInput(window);

		//Imgui
		InitImGuiFrame();
		
		//Rendering commands
		context.deltaTime = deltaTime;
		context.Update();

		//ImGui
		RenderImGui();

		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Terminate and clean all GLFW's & ImGui resources allocated when we exit the render loop.
	FinishImGui();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

GLFWwindow* InitContext() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create the window and set the context to it.
	GLFWwindow* window = glfwCreateWindow(OGL_WIDTH, OGL_HEIGHT, "ENGINE", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	//We initialize glew. GLEW sets the pointer functions for your platform.
	//OpenGL nedds to be initialized by this point. Here that is done in glfwMakeContextCurrent.
	context.InitGLEW();

	//We want the image not to resize inside the imgui window, so we dont need the resize callback
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);
	glfwSetCursorPosCallback(window, Mouse_callback);
	glfwSetScrollCallback(window, Scroll_callback);
	glfwSetKeyCallback(window, Key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

void InitImGui(GLFWwindow* window) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void InitImGuiFrame() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void FinishImGui() {
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void RenderImGui() {

	//ImGui::ShowDemoWindow();

	SetImGuiWindows();

	//Window renders
	r_panel.OnImGuiRender();

	h_panel.OnImGuiRender();

	i_panel.SetSelectedNode(h_panel.GetSelectedNode());
	i_panel.SetSelected(h_panel.isSelected());
	i_panel.OnImGuiRender();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void SetImGuiWindows() {

	ImGui::DockSpaceOverViewport(); // Docking into the base glfw window

	//Render window
	r_panel.SetContext(&context);
	r_panel.SetRenderImage(context.GetRenderTexture());
	
	//Hierarchy panel
	h_panel.SetContext(&context);

	//Inspector panel
	i_panel.SetContext(&context);

}

void Framebuffer_size_callback(GLFWwindow* window, int width, int height){

	glViewport(0, 0, WIDTH, HEIGHT);
}

void Mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn){

	if (moveMouse) {
		float xPos = static_cast<float>(xPosIn);
		float yPos = static_cast<float>(yPosIn);

		if (firstMouse) {
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		float xOffset = (xPos - lastX);
		float yOffset = (lastY - yPos); //Reversed since y-coords range from bottom to top

		lastX = xPos;
		lastY = yPos;

		context.camera.ProcessMouseMovement(xOffset, yOffset);
	}
}

void Scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	context.camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

//Key pressed only once
void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		if (!moveMouse) {
			moveMouse = true;
		}
		else {
			moveMouse = false;
		}
	}
}

void ProcessInput(GLFWwindow* window){

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		context.camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		context.camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		context.camera.ProcessKeyboard(LEFT, deltaTime);
	}
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		context.camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}
