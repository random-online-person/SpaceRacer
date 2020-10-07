#pragma once

#include"stb_image.h"

#include"Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include<iostream>
#include<vector>

class Model
{
public:
	Model(const char* path);
	~Model();

	int GetMeshSize();
	int GetMeshIndexSize(int i);
	unsigned int GetVAO(int i);
	unsigned int GetTextureID(int i);

	void Draw(Shader& shader);

private:

	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};
