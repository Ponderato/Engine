#include <GL/glew.h>
#include <glfw3.h>
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <stdio.h>
#include <vector>

std::vector<std::string> lines;
const char** content;
size_t size;

//Data
//glm::mat4 model_M = glm::mat4(1.0f);
glm::mat4 view_M = glm::mat4(1.0f);
glm::mat4 proj_M = glm::mat4(1.0f);

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

GLuint VBO;
GLuint VAO;
GLuint shaderProgram;

//GLuint EBO;

GLuint texture1, texture2;

float faceVisibility = 0.2;

int width, height, nrChannels;

//Declaration of methods -> C programming stuff :D
GLFWwindow* initContext();
void initData();
void initShaders();
void render();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(){
	GLFWwindow* window = initContext();
	initData();
	initShaders();

	//render loop
	while (!glfwWindowShouldClose(window)) {

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

	//initGLEW();
	//We initialize glew. GLEW sets the pointer functions for your platform.
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("Error: %s\n", glewGetErrorString(err));
		exit(-1);
	}

	//Viewport coordinates and callback function for resize.
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//TODO -> move initOpenGL stuff to a new method void initOGL();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	return window;
}

void initData() {

	//Rectangle
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Copy our index array in an element buffer for OpenGL to use
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Unbind VAO
	glBindVertexArray(0);

	//Textures
	stbi_set_flip_vertically_on_load(true);
	//texture1
	unsigned char* data1 = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data1){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//Free the image memory -> Good practice
	stbi_image_free(data1);

	//texture2
	unsigned char* data2 = stbi_load("face.png", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data2);

	//Bind textures to the corresponding texture unit
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Matrices
	//model_M = glm::rotate(model_M, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
	view_M = glm::translate(view_M, glm::vec3(0.0, 0.0, -3.0));
	proj_M = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
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

void initShaders() {
	GLuint vs = createShader("vertexShader.vs", GL_VERTEX_SHADER);
	GLuint fs = createShader("fragmentShader.fs", GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);

	glLinkProgram(shaderProgram);
	//can also check if linking a shader program failed.

	//Uniforms -> AFTER glUseProgram(); !!!!!!!!!
	//uniforms need the program to be linked to the context to 
	glUseProgram(shaderProgram);
	//the final number -> texture unit each uniform sampler correpsonds to
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

	//location, how many matrices we want to pass, transpose, data as glm::value_ptr
	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewM"), 1, GL_FALSE, glm::value_ptr(view_M));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projM"), 1, GL_FALSE, glm::value_ptr(proj_M));
}

void render() {
	glm::mat4 model_M = glm::mat4(1.0f);
	model_M = glm::rotate(model_M, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5, 1.0, 0.0));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);

	//this goes here in order to the arrow keys to work 
	glUniform1f(glGetUniformLocation(shaderProgram, "faceV"), faceVisibility);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelM"), 1, GL_FALSE, glm::value_ptr(model_M));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

//Checks if esc key have been pressed and if so, "closes" the window
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
}
