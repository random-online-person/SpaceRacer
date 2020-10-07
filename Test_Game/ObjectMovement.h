#pragma once

#include"glew.h"
#include"glfw3.h"

#include"glm.hpp"
#include"gtc/matrix_transform.hpp"

#include<iostream>

class ObjectMovement
{
public:
	ObjectMovement();
	~ObjectMovement();

	void UpdateObject(GLFWwindow* window, const float deltatime, const double& offsetX, const double& offsetY);
	void setScale(glm::vec3 scale);

	glm::mat4 GetModelMat();

	glm::vec3 GetPosition();
	float GetYaw();
	float GetPitch();

	bool IsBoostOn();

private:

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_worldup;


	glm::vec3 m_velocity;

	glm::mat4 m_translation;
	glm::vec3 m_scale;

	float m_yaw;
	float m_pitch;
	float m_roll;

	float m_boostTimer;
	float m_boostCooldown;
	bool m_boostOn;
	bool m_canBoost;

	float m_acceleration;
	float m_sensitivity;

	void UpdateMovement(GLFWwindow* window);
	void UpdateVec(const float deltatime, const double& offsetX, const double& offsetY);

};

