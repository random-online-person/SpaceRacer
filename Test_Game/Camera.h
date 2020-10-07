#pragma once

#include"glew.h"
#include"glfw3.h"
#include "glm.hpp"
#include"gtc/matrix_transform.hpp"

#include <iostream>

class Camera
{
public:

	Camera(glm::vec3 posititon, glm::vec3 camerafront, glm::vec3 cameraup, float RenderDis);
	~Camera();

	glm::mat4 GetViewMat();
	glm::mat4 GetProjectionMat();

	void Update(GLFWwindow* window , const float deltatime, const double& offsetX, const double& offsetY);
	void Follow(glm::vec3 objectposition, float yaw, float pitch, float offsetDis, float offsetyaw, float offsetpitch);

	glm::vec3 getPosition();
	glm::vec3 getFront();

private:

	float m_yaw;
	float m_pitch;
	float m_roll;

	float m_sensitivity;
	float m_movementSpeed;

	float m_renderDistance;

	glm::vec3 m_CameraPosition;
	glm::vec3 m_CameraFront;
	glm::vec3 m_CameraUp;
	glm::vec3 m_CameraRight;
	glm::vec3 m_WorldUp;

	glm::mat4 m_View;
	glm::mat4 m_Projection;

	void UpdateMat();
	void UpdateVec(const float deltatime, const double& offsetX, const double& offsetY);
	void UpdateMovement(GLFWwindow* window);
};

