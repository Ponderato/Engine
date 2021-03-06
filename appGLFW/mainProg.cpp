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
#include "ShaderEditorPanel.h"

//Data
const unsigned int OGL_WIDTH = 1500;
const unsigned int OGL_HEIGHT = 600;

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

static bool moveMouse = false;
bool firstMouse = true;
const float sensitivity = 0.1f;
float lastX = OGL_WIDTH / 2;
float lastY = OGL_HEIGHT / 2;

const unsigned int N_LIGHTS = 3;

//ForFPS
float deltaTime = 0.0f;//Time between current frame and last frame
float lastFrame = 0.0f;//Time of last frame
unsigned int counter = 0;
double fps = 0;
double ms = 0;

//For Movement
float deltaTime1 = 0.0f;	// Time between current frame and last frame
float lastFrame1 = 0.0f; // Time of last frame
bool moveCamera = false;

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
ShaderEditorPanel se_panel;

//Declaration of methods -> C programming stuff :D
GLFWwindow* InitContext();
void InitImGui(GLFWwindow* window);
void InitImGuiFrame();
void FinishImGui();
void RenderImGui();
void SetImGuiWindows();

void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void Window_size_callback(GLFWwindow* window, int width, int height);
void Mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void Scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* window);

int main(){

	//Initialization
	GLFWwindow* window = InitContext();

	context.InitOGL();
	InitImGui(window);

	context.InitShaders("forward_vs.glsl", "forward_fs.glsl");            //programs[0]
	context.InitShaders("lightBox_vs.glsl", "lightBox_fs.glsl");		  //programs[1]
	context.InitShaders("geometryPass_vs.glsl", "geometryPass_fs.glsl");  //programs[2]
	context.InitShaders("lightingPass_vs.glsl", "lightingPass_fs.glsl");  //programs[3]
	context.InitShaders("frustum_vs.glsl", "frustum_fs.glsl");			  //programs[4]

	#pragma region INITGEO
	//../ refers to the parent folder, so we need two of them to get to the textures folder
	context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", "../../textures/container2_emissive.jpg", cubePositions[0], context.parentNode);
	context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[1], context.nodes[0]);
	for (int i = 2; i < 7; i++) {
		context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[i], context.parentNode);
	}

	context.InitModel("../../models/shiba/shiba.obj", glm::vec3(2, -2, 0), context.parentNode);
	context.nodes[7]->Scale(glm::vec3(100.0f));

	for (int i = 0; i < 3; i++) {
		context.InitLightCube(lightPos[i], lightColor[i], context.parentNode);
	}
	#pragma endregion

	context.InitCameraSet(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), (WIDTH / HEIGHT),  2.5f, 0.1f, 45.0f, -90.0f, 0.0f, context.parentNode);
	context.InitCamera(glm::vec3(15.0f, 0.0f, -4.0f), glm::vec3(0.0f, 1.0f, 0.0f), (WIDTH / HEIGHT), 2.5f, 0.1f, 45.0f, -180.0f, 0.0f, context.parentNode);
	
	context.SetWIdth(WIDTH);
	context.SetHeight(HEIGHT);
	context.SetDeferredPipeline();
	context.SetForwardPipeline();
	
	glViewport(0, 0, WIDTH, HEIGHT);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
	
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		counter++;
		if (deltaTime >= 1.0 / 10.0) {
			fps = (1.0 / deltaTime) * counter;
			ms = (double)(deltaTime / counter) * 1000;
			lastFrame = currentFrame;
			counter = 0;

		}

		//For input
		currentFrame = glfwGetTime();
		deltaTime1 = currentFrame - lastFrame1;
		lastFrame1 = currentFrame;

		//Input
		ProcessInput(window);

		//Imgui
		InitImGuiFrame();
		
		//Rendering commands
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

	glfwSetWindowSizeCallback(window, Window_size_callback);
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

	se_panel.ConfigureEditor();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		//style.Colors[ImGuiCol_TitleBg] = ImVec4(1, 1, 0, 1);
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
	r_panel.SetFPS((int)fps);
	r_panel.SetMS(ms);

	h_panel.OnImGuiRender();

	i_panel.SetSelectedNode(h_panel.GetSelectedNode());
	i_panel.SetSelected(h_panel.isSelected());
	i_panel.OnImGuiRender();

	h_panel.SetSelected(i_panel.isSelected());

	se_panel.OnImGuiRender();


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
	if(context.GetActivePipe()->Name == "Deferred")
		r_panel.SetRenderImage(context.GetRenderTextureD());
	else
		r_panel.SetRenderImage(context.GetRenderTextureF());
	
	//Hierarchy panel
	h_panel.SetContext(&context);

	//Inspector panel
	i_panel.SetContext(&context);

	//ShaderEditor panel
	se_panel.SetContext(&context);

}

void Window_size_callback(GLFWwindow* window, int width, int height) {

	if (width < 1000)
		width = 1000;

	if (height < 600)
		height = 600;

	glfwSetWindowSize(window, width, height);
}

void Mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn){

	if (r_panel.IsFocused() && moveMouse) {

		float xPos = static_cast<float>(xPosIn);
		float yPos = static_cast<float>(yPosIn);


		float xOffset = (xPos - lastX);
		float yOffset = (lastY - yPos); //Reversed since y-coords range from bottom to top

		lastX = xPos;
		lastY = yPos;

		context.GetActiveCamera()->ProcessMouseMovement(xOffset, yOffset);
	}
}

void Scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	if (r_panel.IsFocused() && moveMouse)
		context.GetActiveCamera()->ProcessMouseScroll(static_cast<float>(yOffset));
}

//Key pressed only once
void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		if (!moveMouse) {
			moveMouse = true;
		}
		else {
			moveMouse = false;
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			lastX = (float)xpos;
			lastY = (float)ypos;
		}
	}
}

void ProcessInput(GLFWwindow* window){

	if (r_panel.IsFocused()) {

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			context.GetActiveCamera()->ProcessKeyboard(FORWARD, deltaTime1);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			context.GetActiveCamera()->ProcessKeyboard(BACKWARD, deltaTime1);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			context.GetActiveCamera()->ProcessKeyboard(LEFT, deltaTime1);
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			context.GetActiveCamera()->ProcessKeyboard(RIGHT, deltaTime1);
		}
	}
}
