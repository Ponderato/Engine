#pragma once

#include <GL/glew.h>
#include <glfw3.h>

#include <Context.h>

//Data
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

bool firstMouse = true;
const float sensitivity = 0.1f;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;


float deltaTime = 0.0f;//Time between current frame and last frame
float lastFrame = 0.0f;//Time of last frame

//Light & cube data
glm::vec3 lightPos[3] = {
glm::vec3(2.0f, 2.0f, 0.0f),
glm::vec3(-2.0f, -3.0f, -4.0f),
glm::vec3(1.5f, 1.0f, -6.0f)
};
glm::vec3 lightColor[3] = {
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

//Declaration of methods -> C programming stuff :D
GLFWwindow* initContext();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

int main(){

	GLFWwindow* window = initContext();
	context.InitOGL();

	context.InitShaders("default_vs.glsl", "default_fs.glsl");            //programs[0]
	context.InitShaders("lightBox_vs.glsl", "lightBox_fs.glsl");		  //programs[1]
	context.InitShaders("geometryPass_vs.glsl", "geometryPass_fs.glsl");  //programs[2]
	context.InitShaders("lightingPass_vs.glsl", "lightingPass_fs.glsl");  //programs[3]

	context.InitCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 0.1f, 45.0f, -90.0f, 0.0f);

	//../ refers to the parent folder, so we need two of them to get to the textures folder
	context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", "../../textures/container2_emissive.jpg", cubePositions[0], glm::vec3(1.0f), glm::vec4(1.0f, 2.0f, 0.7f, 90.0f), context.parentNode);
	context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[1], glm::vec3(1.0f), glm::vec4(360.0f), context.models[0]);
	for (int i = 2; i < 7; i++) {
		context.InitCube("../../textures/container2.jpg", "../../textures/container2_specular.jpg", cubePositions[i], glm::vec3(1.0f), glm::vec4(360.0f), context.parentNode);
	}

	context.InitModel("../../models/shiba/shiba.obj", glm::vec3(2, -2, 0), glm::vec3(100), glm::vec4(360.0f), context.parentNode);

	for (int i = 0; i < 3; i++) {
		context.InitLightCube(lightPos[i], glm::vec3(0.25f), glm::vec4(360.0f), lightColor[i], context.parentNode);
	}
	
	context.SetWIdth(WIDTH);
	context.SetHeight(HEIGHT);
	context.SetProjectionMatrix(0.1f, 100.f);
	context.SetDefaultPipeline();
	context.SetUniforms();
	
	//render loop
	while (!glfwWindowShouldClose(window)) {
	
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	
		//input
		processInput(window);
	
		//rendering commands
		context.deltaTime = deltaTime;
		context.Update();
	
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ENGINE", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	//We initialize glew. GLEW sets the pointer functions for your platform.
	//OpenGL nedds to be initialized by this point. Here that is done in glfwMakeContextCurrent.
	context.InitGLEW();

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

	context.camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	context.camera.ProcessMouseScroll(static_cast<float>(yOffset));
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
		context.camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		context.camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		context.camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		context.camera.ProcessKeyboard(RIGHT, deltaTime);
	}
		

}
