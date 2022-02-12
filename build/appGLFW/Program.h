#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

class Program 
{
private:
	void checkCompilling(unsigned int shaderID, std::string type);
	const char* readFile(const char* fileName);
	unsigned int createShader(const char* fileName, GLenum shaderType);
public:
	unsigned int ID;

	Program(const char* vertexShaderPath, const char* fragmentShaderPath);
	inline void use() { glUseProgram(ID); }
};

#endif