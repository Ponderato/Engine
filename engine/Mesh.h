#pragma once

#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <string>
#include <vector>

#include "Program.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type; //matDiffuse, matSpecular, matEmissive
	std::string path;
};

class Mesh 
{
public:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);

	void Draw(Program &program);
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void SetUpMeshData();
	void SetUpMeshTextures();
};

#endif 
