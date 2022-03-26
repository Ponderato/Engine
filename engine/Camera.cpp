#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float speed, float sensitivity, float fov, float yaw, float pitch) {
	
	this->position = position;
	this->worldUp = worldUp;

	this->speed = speed;
	this->sensitivity = sensitivity;
	this->fov = fov;
	this->yaw = yaw;
	this->pitch = pitch;

	CalculateVectors();
}

void Camera::CalculateVectors() {

	//Front vector
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);

	//Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::ProcessKeyboard(MovementDir direction, float deltaTime) {

	float vel = speed * deltaTime;
	switch (direction) {
	case FORWARD:
		position += front * vel;
		break;
	case BACKWARD:
		position -= front * vel;
		break;
	case LEFT:
		position -= right * vel;
		break;
	case RIGHT:
		position += right * vel;
		break;
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset) {
	
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f) 
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	CalculateVectors();
}

void Camera::ProcessMouseScroll(float yOffset){

	fov -= (float)yOffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}