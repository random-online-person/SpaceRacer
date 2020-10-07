#pragma once

#include "glew.h"
#include "glfw3.h"

class Input
{
public:

	Input();
	~Input();

	void Update(GLFWwindow* window);


	double GetMouseOffsetX();
	double GetMouseOffsetY();

	float GetDeltaTime();

private:

	float m_LastTime;
	float m_CurrentTime;
	float DeltaTime;

	double m_lastMouseX;
	double m_lastMouseY;
	double m_mouseX;
	double m_mouseY;
	double m_mouseOffsetX;
	double m_mouseOffsetY;
	bool m_firstMouse;
	
	void UpdateTime();
	void UpdateMouse(GLFWwindow* window);

};

