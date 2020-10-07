#pragma once
#include"glew.h"
#include"glfw3.h"
#include"glm.hpp"
#include"gtc\type_ptr.hpp"

#include<iostream>
#include<string>
#include<fstream>

class Shader{

public:

	Shader(const char* VertexShader, const char* FragmentShader);
	~Shader();

	void Use();
	void UnBind();

	int GetID();

	void setInt(const char* name, int value);
	void setF1(const char* name, float value);
	void setVec3(const char* name, glm::fvec3 value);
	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE); 

private:
	
	unsigned int m_ID;

	std::string GetShader(const char* filepath);
};

