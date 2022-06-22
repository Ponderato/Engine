#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <GL/glew.h>
#include <glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Program 
{
public:
	unsigned int ID;

	Program(const char* vertexShaderPath, const char* fragmentShaderPath);

	inline void Use() { glUseProgram(ID); }

	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, const glm::vec3 value) const;
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