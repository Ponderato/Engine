#include "Mesh.h"
#include <GL/glew.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices) {
	
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;

	setUpMesh();
}

void Mesh::setUpMesh() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Unbind VAO
	glBindVertexArray(0);
}

void Mesh::Draw(Program &program) {

	unsigned int diffuseNumber = 1;
	unsigned int specularNumber = 1;
	unsigned int emissiveNumber = 1;

	for (int i = 0; i < textures.size(); i++) {
		
		glActiveTexture(GL_TEXTURE0 + i);

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
		glUniform1i(glGetUniformLocation(program.ID, (name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	//Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//Set everything back to default
	glActiveTexture(GL_TEXTURE0);
}