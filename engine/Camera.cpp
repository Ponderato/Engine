#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float speed, float sensitivity, float fov, float yaw, float pitch, Node* parent) {
	
	this->transform.position = position;
	this->worldUp = worldUp;

	this->speed = speed;
	this->sensitivity = sensitivity;
	this->fov = fov;
	this->yaw = yaw;
	this->pitch = pitch;

	this->near = 0.1f;
	this->far = 100.f;

	Parent(parent);
	this->tag = "Camera";

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
		transform.position += front * vel;
		break;
	case BACKWARD:
		transform.position -= front * vel;
		break;
	case LEFT:
		transform.position -= right * vel;
		break;
	case RIGHT:
		transform.position += right * vel;
		break;
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset) {
	
	xOffset = xOffset * sensitivity;
	yOffset = yOffset * sensitivity;

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

void Camera::Resize() {
	
	projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);

	projectionMatrix[0].x = 1.0f / tan(glm::radians(30.0f));
	projectionMatrix[1].y = aspectRatio / tan(glm::radians(30.0f));
	projectionMatrix[2].z = -(far + near) / (far - near);
	projectionMatrix[3].z = -2.0f * far * near / (far - near);
	projectionMatrix[2].w = -1.0f;
}