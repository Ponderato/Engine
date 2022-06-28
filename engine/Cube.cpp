#include "Cube.h"

Cube::Cube(std::string diffuse, std::string specular, std::string emissive, glm::vec3 position, Node* parent) : Model(){
	
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
	textures.push_back({ NULL, "matDiffuse", diffuse });
	textures.push_back({ NULL, "matSpecular", specular });
	textures.push_back({ NULL, "matEmissive", emissive });

	//Transform
	this->transform.position = position;

	//We want to render the cubes
	this->renderable = true;
	this->hasTex = true;
	this->hasEmissive = true;
	this->hasSpecular = true;

	Parent(parent);
	this->tag = "Cube";


	if (TexOK(textures)) {
		mesh = new Mesh(vertices, textures, indices);
	}
	else {
		this->hasTex = false;
		mesh = new Mesh(vertices, indices);
	}
}



Cube::Cube(std::string diffuse, std::string specular, glm::vec3 position, Node* parent) : Model() {

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
	textures.push_back({ NULL, "matDiffuse", diffuse });
	textures.push_back({ NULL, "matSpecular", specular });

	this->transform.position = position;

	this->renderable = true;
	this->hasTex = true;
	this->hasSpecular = true;

	Parent(parent);
	this->tag = "Cube";

	if (TexOK(textures)) {
		mesh = new Mesh(vertices, textures, indices);
	}else {
		this->hasTex = false;
		mesh = new Mesh(vertices, indices);
	}
}

Cube::Cube(std::string diffuse, glm::vec3 position, Node* parent) : Model() {

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
	textures.push_back({ NULL, "matDiffuse", diffuse });

	this->transform.position = position;

	this->renderable = true;
	this->hasTex = true;

	Parent(parent);
	this->tag = "Cube";

	if (TexOK(textures)) {
		mesh = new Mesh(vertices, textures, indices);
	}
	else {
		this->hasTex = false;
		mesh = new Mesh(vertices, indices);
	}
}

Cube::Cube(glm::vec3 position, Node* parent) : Model() {

	std::vector<Vertex> vertices;
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

	this->transform.position = position;
	this->renderable = true;

	Parent(parent);
	this->tag = "Cube";

	mesh = new Mesh(vertices, indices);
}

void Cube::Draw(Program& program){
	mesh->Draw(program);
}

bool Cube::TexOK(std::vector<Texture> textures) {
	
	int width, height, nrChannels;
	for (Texture tex : textures) {
		unsigned char* data = stbi_load(tex.path.c_str(), &width, &height, &nrChannels, 0);
		if (!data) {
			return false;
		}
	}

	return true;
}

void Cube::Aux() {

}