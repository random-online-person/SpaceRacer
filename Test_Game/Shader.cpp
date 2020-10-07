#include "Shader.h"

Shader::Shader(const char* VertexShader, const char* FragmentShader) {

	std::string temp = GetShader(VertexShader);
	const char* vertexShaderSource = temp.c_str();

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	temp = GetShader(FragmentShader);
	const char* fragmentShaderSource = temp.c_str();
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);
	glLinkProgram(m_ID);
	// check for linking errors
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

Shader::~Shader() {

	glDeleteProgram(m_ID);
}

void Shader::Use() {

	glUseProgram(m_ID);
}

void Shader::UnBind() {

	glUseProgram(0);
}

std::string Shader::GetShader(const char* filepath) {
	
	std::string text, temp;
	std::fstream shader(filepath);

	while (std::getline(shader, temp)) {
		text += temp + "\n";
	}

	if (text.size() == 0) {
		std::cout << "ERROR :: Shader file Error" << std::endl << filepath << std::endl;
	}

	return text;

}

int Shader::GetID() {
	return m_ID;
}

void Shader::setInt(const char* name, int value) {
	glUniform1i(glGetUniformLocation(m_ID, name), value);
}

void Shader::setF1(const char* name, float value) {

	glUniform1f(glGetUniformLocation(m_ID, name), value);
}

void Shader::setVec3(const char* name, glm::fvec3 value) {

	glUniform3fv(glGetUniformLocation(m_ID, name), 1, glm::value_ptr(value));
}

void Shader::setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, transpose, glm::value_ptr(value));
}
