#pragma once

#include"glm.hpp"

#include"Shader.h"


#include <iostream>
#include <map>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

struct Texture
{
	unsigned int ID;
	std::string type;
	std::string path;
};

class Mesh {

public:

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();

	unsigned int GetVAO();
	
	void Draw(Shader& shader);

private:

	unsigned int VAO, VBO, EBO;
	void setupMesh();
};
