#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <string>
#include <vector>

#include "Program.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type; //diffuse, specular, emissive
	std::string path;
};

class Mesh 
{
public:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);

	void Draw(Program &program);
private:
	unsigned int VBO;
	unsigned int EBO;

	void setUpMesh();
};

#endif 
