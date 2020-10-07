#pragma once

#include"glew.h"
#include"glfw3.h"

#include"Shader.h"
#include"Camera.h"
#include"stb_image.h"

#include<iostream>
#include<vector>
#include<string>

class SkyBox {

public:

	SkyBox(std::vector<std::string> faces);
	~SkyBox();

	void AttachShader(Shader& shader);
	void Draw(Shader& shader, Camera& camera);

	unsigned int GetTexture();
	unsigned int GetVAO();


private:

	unsigned int m_textureID;
	unsigned int m_skyboxVAO;
	unsigned int m_skyboxVBO;

	void loadCubemap(std::vector<std::string> faces);
};
