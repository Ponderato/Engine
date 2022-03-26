#include "Model.h"
#include <iostream>
#include "stb_image.h"

Model::Model(const std::string &path) {
	loadModel(path);
};

void Model::draw(Program& program) {

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(program);
	}
}

void Model::loadModel(std::string path) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//Checks if the scene and the root node of the scene are not null and check one of its 
	//flags to see if the returned data is incomplete.
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	} 

	//Directory path.
	directory = path.substr(0, path.find_last_of('/'));

	//Process root node recursively.
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	
	//process the node's meshes (if any)
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//do the same for the children
	for (int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	//Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		//this vector is used as a placeholder to translate assimp data.
		glm::vec3 auxVector;

		//positions
		auxVector.x = mesh->mVertices[i].x;
		auxVector.y = mesh->mVertices[i].y;
		auxVector.z = mesh->mVertices[i].z;
		vertex.pos = auxVector;

		//normals
		if (mesh->HasNormals()) {
			auxVector.x = mesh->mNormals[i].x;
			auxVector.y = mesh->mNormals[i].y;
			auxVector.z = mesh->mNormals[i].z;
			vertex.normal = auxVector;
		}

		//texCoords
		if (mesh->mTextureCoords[0]) { //does the mesh contain texture coordinates?
			glm::vec2 auxVec;

			//A vertex can contain up to 8 different texture coordinates. 
			//We thus make the assumption that we won't use models where a 
			//vertex can have multiple texture coordinates so we always take 
			//the first set (0).
			auxVec.x = mesh->mTextureCoords[0][i].x;
			auxVec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = auxVec;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	//Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++ ) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//Materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMats = loadTextures(material, aiTextureType_DIFFUSE, "matDiffuse");
	textures.insert(textures.end(), diffuseMats.begin(), diffuseMats.end());

	std::vector<Texture> specularMats = loadTextures(material, aiTextureType_SPECULAR, "matSpecular");
	textures.insert(textures.end(), specularMats.begin(), specularMats.end());

	std::vector<Texture> emissiveMats = loadTextures(material, aiTextureType_EMISSIVE, "matEmissive");
	textures.insert(textures.end(), emissiveMats.begin(), emissiveMats.end());

	return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType type, std::string typeName) {
	
	//Note that we make the assumption that texture file paths in model files 
	//are local to the actual model object e.g.in the same directory as the 
	//location of the model itself.
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = loadTexFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int Model::loadTexFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	//filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
