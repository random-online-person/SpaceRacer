#include "ObjectMovement.h"

ObjectMovement::ObjectMovement() {

	m_position = glm::vec3(0.f, 0.f, 0.f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_worldup = glm::vec3(0.0f, 1.0f, 0.0f);
	m_scale = glm::vec3(1.f);

	m_velocity = glm::vec3(0.0f, 0.f, 0.f);

	m_boostOn = false;


	m_yaw = 0.f;
	m_pitch = 0.f;
	m_roll = 0.f;

	m_acceleration = 40.f;
	m_sensitivity = 10.f;
}

ObjectMovement::~ObjectMovement() {

}

void ObjectMovement::UpdateObject(GLFWwindow* window, const float deltatime, const double& offsetX, const double& offsetY) {
	UpdateVec(deltatime, offsetX, offsetY);
	UpdateMovement(window);
}

void ObjectMovement::setScale(glm::vec3 scale) {

	m_scale = scale;
}

void ObjectMovement::UpdateMovement(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//m_position += m_front * m_acceleration;
		//m_velocity += m_front * m_acceleration;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		//m_position -= m_front * m_acceleration;
		//m_velocity -= m_front * m_acceleration;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		m_roll += 3.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		m_roll -= 3.f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_boostOn = true;
	}
	else {
		m_boostOn = false;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		//std::cout << m_yaw << " " << m_pitch << std::endl;
		//std::cout << m_front.x << " " << m_front.y << " " << m_front.z << std::endl;
		std::cout << m_position.x << " " << m_position.y << " " << m_position.z << std::endl;
	}
}

void ObjectMovement::UpdateVec(const float deltatime, const double& offsetX, const double& offsetY) {
	//std::cout << offsetX << " " << offsetY << std::endl;
	m_yaw += offsetX * deltatime * m_sensitivity;
	m_pitch += offsetY * deltatime * m_sensitivity;

	if (m_pitch > 89.0f) {
		m_pitch = 89.0f;
	}
	else if (m_pitch < -89.0f) {
		m_pitch = -89.0f;
	}	  

	if (m_yaw > 89.0f) {
		m_yaw = 89.0f;
	}
	else if (m_yaw < -89.0f) {
		m_yaw = -89.0f;
	}

	glm::vec3 Direction;
	Direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	Direction.y = sin(glm::radians(m_pitch));
	Direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(Direction);
	//m_front = glm::normalize(glm::vec3(abs(m_pitch), abs(m_yaw), 0));
	m_right = glm::normalize(glm::cross(m_front, m_worldup));
	m_up = glm::normalize(glm::cross(m_right, m_front));

	if (m_boostOn) {
		m_position += m_front * m_acceleration * 3.f;
	}
	else {
		m_position += m_front * m_acceleration;
	}

	
	//m_position += m_velocity * deltatime;

	//std::cout << m_front.x << " " << m_front.y << " " << m_front.z << std::endl;
}

//-4.32084e-08 0.151261 - 0.988494

//-4.36664e-08 - 0.045363 - 0.998971

glm::mat4 ObjectMovement::GetModelMat() {
	m_translation = glm::mat4(1.0);
	
	m_translation = glm::translate(m_translation, m_position);
	
	m_translation = glm::rotate(m_translation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_translation = glm::rotate(m_translation, glm::radians(-m_roll), glm::vec3(0.f, 0.f, 1.f));
	m_translation = glm::rotate(m_translation, glm::radians(-m_yaw), glm::vec3(0.f, 1.f, 0.f));
	m_translation = glm::rotate(m_translation, glm::radians(-m_pitch), glm::vec3(1.f, 0.f, 0.f));
	
	//m_translation = glm::rotate(m_translation, glm::radians(1.0f), m_front);
	//m_translation = glm::rotate(m_translation, glm::radians(m_yaw), m_front);
	//m_translation = glm::rotate(m_translation, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	//m_translation = glm::rotate(m_translation, glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	
	m_translation = glm::scale(m_translation, m_scale);

	//std::cout << m_pitch << std::endl;
	return m_translation;
}

glm::vec3 ObjectMovement::GetPosition() {
	return m_position;
}

float ObjectMovement::GetYaw() {
	return m_yaw;
}

float ObjectMovement::GetPitch() {
	return m_pitch;
}

bool ObjectMovement::IsBoostOn() {
	return m_boostOn;
}