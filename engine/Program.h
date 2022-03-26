#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <GL/glew.h>

class Program 
{
public:
	unsigned int ID;

	Program() {} //Need it in order to create the array before passing the shaders
	Program(const char* vertexShaderPath, const char* fragmentShaderPath);

	inline void Use() { glUseProgram(ID); }

	void SetInt(const std::string &name, int value) const;

	void SetVec3(const std::string& name, const glm::vec3 value) const;
	void SetMultipleVec3(const std::string& name, int count, const glm::vec3 value[]) const;

	void SetMat4(const std::string& name, const glm::mat4 value) const;

private:
	std::vector<std::string> lines;
	const char** content;
	size_t size;

	void CheckCompilling(unsigned int shaderID, std::string type);
	void ReadFile(const char* fileName);
	unsigned int CreateShader(const char* fileName, GLenum shaderType);

};

#endif