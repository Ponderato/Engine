#include "Mesh.h"
#include <GL/glew.h>
#include "stb_image.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices) {
	
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;

	SetUpMeshData();
	SetUpMeshTextures();
}

void Mesh::SetUpMeshData() {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	//Indices
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Unbind VAO
	glBindVertexArray(0);
}

void Mesh::SetUpMeshTextures() {

	int width, height, nrChannels;

	for (int i = 0; i < textures.size(); i++) {
		//c_str() to transform a std::string into a const char*
		unsigned char* data = stbi_load(textures[i].path.c_str(), &width, &height, &nrChannels, 0);

		glGenTextures(1, &textures[i].id);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);

		//Set to default
		glActiveTexture(GL_TEXTURE0);
	}
}

void Mesh::Draw(Program &program) {

	//We do this here instead of in SetUpMeshTextures because here
	//we have the program. 

	unsigned int diffuseNumber = 1;
	unsigned int specularNumber = 1;
	unsigned int emissiveNumber = 1;

	for (int i = 0; i < textures.size(); i++) {
		
		//glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = textures[i].type;
		if (name == "matDiffuse") {
			number = std::to_string(diffuseNumber++); //++ at the end means
			//that the actual diffuseNumber is stored in number and then it
			//increments for the next round
		}
		else if (name == "matSpecular") {
			number = std::to_string(specularNumber++);
		}
		else if (name == "matEmissive") {
			number = std::to_string(emissiveNumber++);
		}

		//set sampler to the correct texture unit
		program.SetInt((name + number).c_str(), i);

		//glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	//Draw mesh
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	//Set everything back to default
	//glActiveTexture(GL_TEXTURE0);
}