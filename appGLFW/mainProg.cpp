#include <gl/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <stdio.h>
#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <engine/oglContext.h>

#include "stb_image.h"

std::vector<std::string> lines;
const char** content;
size_t size;

//bool firstMouse = true;
const float sensitivity = 0.1f;
float lastX = 400;
float lastY = 300;
float yaw = -90.0f;//Camera pointing towards negative z axis.
float pitch = 0.0f;

float deltaTime = 0.0f;//Time between current frame and last frame
float lastFrame = 0.0f;//Time of last frame

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

float faceVisibility = 0.2;

float fov = 45.0f;

//Data

glm::mat4 view_M;
glm::mat4 proj_M = glm::mat4(1.0f);

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f, //0
	 0.5f, -0.5f, -0.5f, //1
	 0.5f,  0.5f, -0.5f, //2
	-0.5f,  0.5f, -0.5f, //3

	-0.5f, -0.5f,  0.5f, //4
	 0.5f, -0.5f,  0.5f, //5
	 0.5f,  0.5f,  0.5f, //6
	-0.5f,  0.5f,  0.5f, //7

	-0.5f,  0.5f,  0.5f, //8
	-0.5f,  0.5f, -0.5f, //9
	-0.5f, -0.5f, -0.5f, //10
	-0.5f, -0.5f,  0.5f, //11

	 0.5f,  0.5f,  0.5f, //12
	 0.5f,  0.5f, -0.5f, //13
	 0.5f, -0.5f, -0.5f, //14
	 0.5f, -0.5f,  0.5f, //15

	-0.5f, -0.5f, -0.5f, //16
	 0.5f, -0.5f, -0.5f, //17
	 0.5f, -0.5f,  0.5f, //18
	-0.5f, -0.5f,  0.5f, //19

	-0.5f,  0.5f, -0.5f, //20
	 0.5f,  0.5f, -0.5f, //21
	 0.5f,  0.5f,  0.5f, //22
	-0.5f,  0.5f,  0.5f, //23
};
float cubeTexCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};
const unsigned int cubeTriangleIndex[] = {
	0, 1, 2,
	0, 2, 3,

	4, 5, 6,
	4, 6, 7,

	8, 9, 10,
	8, 10, 11,

	12, 13, 14,
	12, 14, 15,

	16, 17, 18,
	16, 18, 19,

	20, 21, 22,
	20, 22, 23
};

const int SHADERS_COUNT = 2;

GLuint VAO;
GLuint lightVAO;
GLuint vertexVBO;
GLuint textureVBO;
GLuint EBO;
GLuint shaderProgram[SHADERS_COUNT];

GLuint texture1, texture2;

int width, height, nrChannels;

//Declaration of methods -> C programming stuff :D
GLFWwindow* initContext();
void initData();
void initShaders(const char* vertexShader, const char* fragmentShader, int i);
void render();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

int main(){
	GLFWwindow* window = initContext();
	initOGL();
	initShaders("vertexShader.vs", "fragmentShader.fs", 0);
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

	return window;
}

void initData() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoords), cubeTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeTriangleIndex), cubeTriangleIndex, GL_STATIC_DRAW);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind VAO
	glBindVertexArray(0);

	//Textures
	stbi_set_flip_vertically_on_load(true);
	//texture1
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//texture2
	data = stbi_load("face.png", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	//Bind textures to the corresponding texture unit
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Uniforms -> AFTER glUseProgram(); !!!!!!!!!
	//uniforms need the program to be linked to the context to 
	glUseProgram(shaderProgram[0]);
	//the final number -> texture unit each uniform sampler correpsonds to
	glUniform1i(glGetUniformLocation(shaderProgram[0], "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram[0], "texture2"), 1);
}

void readFile(const char* filename) {
	FILE* f;
	fopen_s(&f, filename, "rt");
	if (!f) {
		printf("ERROR: Can't open filename: %s\n", filename);
	}
	char line[256];
	while (fgets(line, 256, f)) lines.push_back(line);
	fclose(f);
	size = (int)lines.size();
	content = new const char* [size];
	for (int i = 0; i < size; i++) {
		content[i] = lines[i].c_str();
	}
}

void checkCompilling(GLuint shader) {
	GLint isCompiled = 0;
	GLint length;
	GLchar* log;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = new GLchar[length];
		glGetShaderInfoLog(shader, length, &length, log);
		printf("ERROR: Compile error: %s\n", log);
		delete log;
	}
}

GLuint createShader(const char* fileName, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	readFile(fileName);

	glShaderSource(shader, size, content, NULL);
	glCompileShader(shader);

	delete[] content;
	lines.clear();

	checkCompilling(shader);

	return shader;
}

void initShaders(const char* vertexShader, const char* fragmentShader, int i) {
	GLuint vs = createShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fs = createShader(fragmentShader, GL_FRAGMENT_SHADER);

	shaderProgram[i] = glCreateProgram();

	glAttachShader(shaderProgram[i], vs);
	glAttachShader(shaderProgram[i], fs);

	glLinkProgram(shaderProgram[i]);
	//can also check if linking a shader program failed.
}

void render() {
	static float angle = 0.0f;
	angle = (angle > 360) ? 0 : angle + 0.01f;

	view_M = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

	proj_M = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

	glm::mat4 model_M = glm::mat4(1.0f);
	model_M = glm::rotate(model_M, (float)angle * glm::radians(50.0f), glm::vec3(0.5, 1.0, 0.0));

	glm::mat4 model_M_2 = glm::mat4(1.0f);
	model_M_2 = glm::translate(model_M_2, glm::vec3(-3.0, 0.0, 0.0));
	model_M_2 = glm::rotate(model_M_2, (float)angle * glm::radians(-45.0f) * 3, glm::vec3(0.0, 1.0, 0.0));
	model_M_2 = glm::rotate(model_M_2, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

	static bool c = false;

	static float t = 0.0F; //creamos t y hacemos que vaya cambiando.
	t = (t < 1) ? t + 0.001f : 0.0f;

	//primera curva.
	if (c == false) {
		model_M_2[3].x = 4.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * 4.0f * t * (1.0f - t) * (1.0f - t) + 3.0f * (-4.0f) * t * t * (1.0f - t) + (-4.0f) * t * t * t;
		//muevo a la derecha el cubo aquí directamente al poner model1[3].x = .....
		model_M_2[3].z = 0.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * (-6.0f) * t * (1.0f - t) * (1.0f - t) + 3.0f * (-6.0f) * t * t * (1.0f - t) + 0.0f * t * t * t;
		if (t >= 1) {
			t = 0.0f;
			c = true;
		}
	}
	//segunda curva.
	if (c == true) {
		model_M_2[3].x = (-4.0f) * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * (-4.0f) * t * (1.0f - t) * (1.0f - t) + 3.0f * 4.0f * t * t * (1.0f - t) + 4.0f * t * t * t;
		model_M_2[3].z = 0.0f * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3.0f * 6.0f * t * (1.0f - t) * (1.0f - t) + 3.0f * 6.0f * t * t * (1.0f - t) + 0.0f * t * t * t;
		if (t >= 1) {
			c = false;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram[0]);

	glUniform1f(glGetUniformLocation(shaderProgram[0], "faceV"), faceVisibility);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram[0], "modelM"), 1, GL_FALSE, glm::value_ptr(model_M));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram[0], "viewM"), 1, GL_FALSE, glm::value_ptr(view_M));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram[0], "projM"), 1, GL_FALSE, glm::value_ptr(proj_M));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawArrays(GL_TRIANGLES, 0, 24);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram[0], "modelM"), 1, GL_FALSE, glm::value_ptr(model_M_2));
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos){

	//if (firstMouse) {
	//	lastX = xPos;
	//	lastY = yPos;
	//	firstMouse = false;
	//}

	float xOffset = (xPos - lastX) * sensitivity;
	//Reversed since y-coords range from bottom to top
	float yOffset = (lastY - yPos) * sensitivity; 
	
	lastX = xPos;
	lastY = yPos;

	yaw += xOffset;
	pitch += yOffset;

	//Avoid screen flipping
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	fov -= (float)yOffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		faceVisibility += 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		faceVisibility -= 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += 2.5f *  deltaTime * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= 2.5f * deltaTime * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		cameraPos -= glm::normalize(glm::cross(cameraFront, glm::vec3(0.0, 1.0, 0.0))) * 2.5f * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, glm::vec3(0.0, 1.0, 0.0))) * 2.5f * deltaTime;
	}
		

}
