#ifndef MODEL_H
#define MODEL_H

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <string>
#include <vector>

#include "Mesh.h"

class Model 
{
public:

	Model(const std::string &path);

	void draw(Program &program);
private:

	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadTextures(aiMaterial *material, aiTextureType type, std::string typeName);
	unsigned int loadTexFromFile(const char* path, const std::string& directory);
};


#endif