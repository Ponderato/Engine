#include <gl/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Program.h"

Program::Program(const char* vertexShaderPath, const char* fragmentShaderPath) {
	
	GLuint vertexShader = createShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = createShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	checkCompilling(ID, "PROGRAM");

	//Delete the shaders as they're now linked into the program and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

unsigned int Program::createShader(const char* fileName, GLenum shaderType) {
	
	unsigned int shader = glCreateShader(shaderType);

	const char* shaderCode = readFile(fileName);

	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	checkCompilling(shader, "SHADER");

	return shader;
}

//Checks shader compilation/linking errors
void Program::checkCompilling(unsigned int identifier, std::string type) {

	int success;
	char info[1024];

	if (type == "PROGRAM") {
		glGetProgramiv(identifier, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(identifier, 1024, NULL, info);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR: " << "\n" << info << "\n -- --------------------------------------------------- --" << std::endl;
		}
	}else{
		glGetShaderiv(identifier, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(identifier, 1024, NULL, info);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR in " << type << " shader: " << "\n" << info << "\n -- --------------------------------------------------- --" << std::endl;
		}
	}
}

const char* Program::readFile(const char* fileName) {
	
	std::ifstream shaderFile;
	//Make sure ifstream objects can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		//Open file
		shaderFile.open(fileName);
		//Read file's buffer contemts into streams
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		//Close file
		shaderFile.close();
		//Convert stream into string
		std::string shaderCode = shaderStream.str();
		//Convert to const char* and return it
		return shaderCode.c_str();

	}catch (std::ifstream::failure& error) {
		std::cout << "ERROR::SHADER::COULD_NOT_READ_FILE " << fileName << ": " << error.what() << std::endl;
	}
}