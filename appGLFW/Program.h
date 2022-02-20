#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <GL/glew.h>

class Program 
{
public:
	unsigned int ID;

	Program() {}
	Program(const char* vertexShaderPath, const char* fragmentShaderPath);

	inline void use() { glUseProgram(ID); }

	void setInt(const std::string &name, int value) const;

	void setVec3(const std::string& name, const glm::vec3 value) const;
	void setMultipleVec3(const std::string& name, int count, const glm::vec3 value[]) const;

	void setMat4(const std::string& name, const glm::mat4 value) const;

private:
	std::vector<std::string> lines;
	const char** content;
	size_t size;

	void checkCompilling(unsigned int shaderID, std::string type);
	void readFile(const char* fileName);
	unsigned int createShader(const char* fileName, GLenum shaderType);

};

#endif