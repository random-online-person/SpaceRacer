#include "Input.h"

Input::Input() {

	m_lastMouseX = 0.0;
	m_lastMouseY = 0.0 ;
	m_mouseX = 0.0;
	m_mouseY = 0.0;
	m_mouseOffsetX = 0.0;
	m_mouseOffsetY = 0.0;
	m_firstMouse = true;

}

Input::~Input() {
	
}

void Input::Update(GLFWwindow* window) {
	UpdateTime();
	UpdateMouse(window);
}

void Input::UpdateTime() {
	m_CurrentTime = (float)glfwGetTime();
	DeltaTime = m_CurrentTime - m_LastTime;
	m_LastTime = m_CurrentTime;
}

void Input::UpdateMouse(GLFWwindow* window) {
	glfwGetCursorPos(window, &m_mouseX, &m_mouseY);

	if (this->m_firstMouse)
	{
		m_lastMouseX = m_mouseX;
		m_lastMouseY = m_mouseY;
		m_firstMouse = false;
	}

	m_mouseOffsetX = m_mouseX - m_lastMouseX;
	m_mouseOffsetY = m_lastMouseY - m_mouseY;

	m_lastMouseX = m_mouseX;
	m_lastMouseY = m_mouseY;
}

double Input::GetMouseOffsetX() {
	return m_mouseOffsetX;
}	  		

double Input::GetMouseOffsetY() {
	return m_mouseOffsetY;
}

float Input::GetDeltaTime() {
	return DeltaTime;
}