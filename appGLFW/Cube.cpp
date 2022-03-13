#include "Cube.h"
#include <glm.hpp>

Cube::Cube(float verts[], float texCoords[], float normals[], const unsigned int inds[], std::string diffuse, std::string specular, std::string emissive) {
	
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	int atVertex = 0; 
	int atNormal = 0;
	int atTexCoord = 0;

	//Vertices
	//sizeof(verts)/sizeof(verts[0] to get the number of elements in the array
	for (int i = 0; i < 24; i++) {
		vertices.push_back({ glm::vec3(verts[atVertex], verts[atVertex + 1], verts[atVertex + 2]),
							 glm::vec3(normals[atNormal], normals[atNormal + 1], normals[atNormal + 2]),
							 glm::vec2(texCoords[atTexCoord], texCoords[atTexCoord + 1]) });

		atVertex += 3;
		atNormal += 3;
		atTexCoord += 2;
	}

	//Indices
	for (int i = 0; i < 24; i++) {
		indices.push_back(inds[i]);
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