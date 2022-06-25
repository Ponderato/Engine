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

	projectionMatrixG = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	this->dirty = true;
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

void Camera::UpdateYaw(float yaw) {

	this->yaw = yaw;
	CalculateVectors();
}

void Camera::UpdatePitch(float pitch) {

	this->pitch = pitch;
	CalculateVectors();
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

void Camera::SetFOV(float fov) {
	
	this->fov = fov;
	projectionMatrixL = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	dirty = true;
}

void Camera::SetAspectRatio(float aspect) {

	this->aspectRatio = aspect;
	projectionMatrixL = glm::perspective(glm::radians(fov), aspect, near, far);
	dirty = true;
}

void Camera::SetNear(float near) {

	this->near = near;
	projectionMatrixL = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	dirty = true;
}

void Camera::SetFar(float far) {

	this->far = far;
	projectionMatrixL = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	dirty = true;
}

void Camera::PMRow1(glm::vec2 row) {

	//projectionMatrixL = glm::perspective(glm::radians(fov), aspectRatio, near, far);

	projectionMatrixL[0].x = row.x;
	projectionMatrixL[2].x = row.y;
	dirty = true;
}

void Camera::UpdatePM() {

	projectionMatrixG = projectionMatrixL;
}