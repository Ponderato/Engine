#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float aspect, float speed, float sensitivity, float fov, float yaw, float pitch, Node* parent) {
	
	this->transform.position = position;
	this->worldUp = worldUp;

	this->aspectRatio = aspect;

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
}

void Camera::SetFOV(float fov) {
	
	this->fov = fov;
	projectionMatrixL = glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

void Camera::SetAspectRatio(float aspect) {

	this->aspectRatio = aspect;
	projectionMatrixL = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::SetNear(float near) {

	this->near = near;
	projectionMatrixL = glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

void Camera::SetFar(float far) {

	this->far = far;
	projectionMatrixL = glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

void Camera::PMRow0(glm::vec2 row) {

	projectionMatrixAux[0].x = row.x;
	projectionMatrixAux[2].x = row.y;
}

void Camera::PMRow1(glm::vec2 row) {

	projectionMatrixAux[1].y = row.x;
	projectionMatrixAux[2].y = row.y;
}

void Camera::PMRow2(glm::vec2 row) {

	projectionMatrixAux[2].z = row.x;
	projectionMatrixAux[3].z = row.y;
}

void Camera::UpdateL() {

	//projectionMatrixL[0].x = 1.0f / tan(glm::radians(30.0f));
	//projectionMatrixL[1].y = aspectRatio / tan(glm::radians(30.0f));
	//projectionMatrixL[2].z = -(far + near) / (far - near);
	//projectionMatrixL[3].z = -2.0f * far * near / (far - near);
	//projectionMatrixL[2].w = -1.0f;

	//SetFOV(2 * atan(glm::radians(1 / projectionMatrixAux[1].y)));

	//SetAspectRatio(projectionMatrixL[0].x / projectionMatrixL[1].y);
}

void Camera::UpdatePM() {

	projectionMatrixG = projectionMatrixL;
}