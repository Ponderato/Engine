#include "Cube.h"
#include <glm.hpp>

Cube::Cube(std::string diffuse, std::string specular, std::string emissive) {
	
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	int atVertex = 0; 
	int atNormal = 0;
	int atTexCoord = 0;

	//Vertices
	//We can write this 24 and the 36 in indices since we pre-know the number of vertices and indices our cube has and will be the same for all cubes.
	for (int i = 0; i < 24; i++) {
		vertices.push_back({ glm::vec3(cubeVertices[atVertex], cubeVertices[atVertex + 1], cubeVertices[atVertex + 2]),
							 glm::vec3(cubeNormals[atNormal], cubeNormals[atNormal + 1], cubeNormals[atNormal + 2]),
							 glm::vec2(cubeTexCoords[atTexCoord], cubeTexCoords[atTexCoord + 1]) });

		atVertex += 3;
		atNormal += 3;
		atTexCoord += 2;
	}

	//Indices
	for (int i = 0; i < 36; i++) {
		indices.push_back(cubeTriangleIndex[i]);
	}

	//Textures
	textures.push_back({ 0, "matDiffuse", diffuse });
	textures.push_back({ 1, "matSpecular", specular });
	textures.push_back({ 2, "matEmissive", emissive });
	
	mesh = new Mesh(vertices, textures, indices);
}

void Cube::Draw(Program& program){
	mesh->Draw(program);
}