

#include "Program.h"

Program::Program(const char* vertexShaderPath, const char* fragmentShaderPath) {

	GLuint vertexShader = CreateShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = CreateShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	CheckCompilling(ID, "PROGRAM");

	//Delete the shaders as they're now linked into the program and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLuint Program::CreateShader(const char* fileName, GLenum shaderType) {
	
	GLuint shader = glCreateShader(shaderType);

    //const char* shaderCode = readFile(fileName);
	ReadFile(fileName);

	glShaderSource(shader, size, content, NULL);
	glCompileShader(shader);

	delete[] content;
	lines.clear();

	CheckCompilling(shader, "SHADER");

	return shader;
}

void Program::ReadFile(const char* fileName) {

	FILE* f;
	fopen_s(&f, fileName, "rt");
	if (!f) {
		printf("ERROR: Can't open filename: %s\n", fileName);
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

//Checks shader compilation/linking errors
void Program::CheckCompilling(unsigned int identifier, std::string type) {

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

//The const at the end makes not possible to the function to change the data of the class
void Program::SetInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Program::SetVec3(const std::string& name, const glm::vec3 vector) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vector.x);
}

void Program::SetMultipleVec3(const std::string& name, int count, const glm::vec3 vector[]) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, &vector[0].x);
}

void Program::SetMat4(const std::string& name, const glm::mat4 matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

