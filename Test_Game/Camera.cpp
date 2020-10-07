#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 camerafront, glm::vec3 cameraup, float renderDis)
	:m_CameraPosition(position), m_CameraFront(camerafront), m_CameraUp(cameraup), m_WorldUp(cameraup), m_renderDistance(renderDis) {

	m_View = glm::mat4(1.0f);
	m_CameraRight = glm::vec3(0.0f);

	m_yaw = -90.f;
	m_pitch = 0.f;
	m_roll = 0.f;

	m_movementSpeed = 0.5f;
	m_sensitivity = 40.f;
}

Camera::~Camera() {

}

void Camera::Follow(glm::vec3 objectposition, float yaw, float pitch, float offsetDis, float offsetyaw, float offsetpitch){
	
	m_CameraPosition.x = objectposition.x - (offsetDis * cos(glm::radians(pitch + offsetpitch)) * cos(glm::radians(yaw + offsetyaw)));
	m_CameraPosition.y = objectposition.y - (offsetDis * sin(glm::radians(pitch + offsetpitch)));
	m_CameraPosition.z = objectposition.z - (offsetDis * cos(glm::radians(pitch + offsetpitch)) * sin(glm::radians(yaw + offsetyaw)));

	glm::vec3 Direction;

	Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Direction.y = sin(glm::radians(pitch));
	Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	m_CameraFront = glm::normalize(-Direction);

	m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
	m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));

	m_View = glm::mat4(1.0f);
	m_View = glm::lookAt(m_CameraPosition, m_CameraPosition - m_CameraFront, m_CameraUp);

	m_Projection = glm::mat4(1.0f);
	m_Projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, m_renderDistance);
}

void Camera::Update(GLFWwindow* window, const float deltatime, const double& offsetX, const double& offsetY) {
	//UpdateVec(deltatime, offsetX, offsetY);
	//UpdateMovement(window);
	UpdateMat();
}

void Camera::UpdateMat() {

	m_View = glm::mat4(1.0f);
	m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp);

	m_Projection = glm::mat4(1.0f);
	m_Projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, m_renderDistance);
}

void Camera::UpdateVec(const float deltatime, const double& offsetX, const double& offsetY) {
	//std::cout << offsetX << " " << offsetY << std::endl;
	m_yaw += offsetX * deltatime * m_sensitivity;
	m_pitch += offsetY * deltatime * m_sensitivity;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 Direction;

	Direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	Direction.y = sin(glm::radians(m_pitch));
	Direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
											 
	m_CameraFront = glm::normalize(Direction);
	m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
	m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
}

void Camera::UpdateMovement(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_CameraPosition += m_movementSpeed * m_CameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_CameraPosition -= m_movementSpeed * m_CameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_movementSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_movementSpeed;
}

glm::vec3 Camera::getPosition(){
	//std::cout << m_CameraPosition.x << " " << m_CameraPosition.y << " " << m_CameraPosition.z << std::endl;
	return m_CameraPosition;
}

glm::vec3 Camera::getFront() {
	return -m_CameraFront;
}

glm::mat4 Camera::GetViewMat() {
	return m_View;
}

glm::mat4 Camera::GetProjectionMat() {
	return m_Projection;
}

