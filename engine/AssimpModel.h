#pragma once

#ifndef ASSIMPMODEL_H
#define ASSIMPMODEL_H

#include "Model.h"

class AssimpModel : public Model
{
public:

	AssimpModel(const std::string &path, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);

	void Draw(Program &program) override;
private:

	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> LoadTextures(aiMaterial *material, aiTextureType type, std::string typeName);
	//unsigned int LoadTexFromFile(const char* path, const std::string& directory);
};


#endif