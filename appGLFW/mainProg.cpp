#pragma once

#include <GL/glew.h>
#include <glfw3.h>

#include <engine/oglContext.h>

//Data
bool firstMouse = true;
const float sensitivity = 0.1f;
float lastX = 400;
float lastY = 300;

float deltaTime = 0.0f;//Time between current frame and last frame
float lastFrame = 0.0f;//Time of last frame

//Declaration of methods -> C programming stuff :D
GLFWwindow* initContext();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

int main(){
	GLFWwindow* window = initContext();
	initOGL();
	//programs[0] = Program("vertexShader2.vert", "fragmentShader2.frag");
	//programs[1] = Program("vertexShaderSun.vert", "fragmentShaderSun.frag");
	initShaders(0, "vertexShader2.vert", "fragmentShader2.frag");
	initShaders(1, "vertexShaderSun.vert", "fragmentShaderSun.frag");
	initData();

	//render loop
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input
		processInput(window);

		//rendering commands
		render();

		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Terminate and clean all GLFW's resources allocated when we exit the render loop.
	glfwTerminate();
	return 0;
}

GLFWwindow* initContext() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create the window and set the context to it.
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	//We initialize glew. GLEW sets the pointer functions for your platform.
	//OpenGL nedds to be initialized by this point. Here that is done in glfwMakeContextCurrent.
	initGLEW();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn){

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

	camera->ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera->ProcessMouseScroll(static_cast<float>(yOffset));
}

void processInput(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		camera->ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
		

}
