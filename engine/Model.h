#ifndef MODEL_H
#define MODEL_H

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <string>
#include <vector>
#include <iostream>

#include "Mesh.h"

class Model 
{
public:

	Model(const std::string &path);

	void Draw(Program &program);
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