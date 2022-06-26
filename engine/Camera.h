#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Node.h"

//An abstraction to stay away from window-system specific input methods
enum MovementDir {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera : public Node
{
public:

	Camera() = default;
	Camera(glm::vec3 position, glm::vec3 worldUp, float speed, float sensitivity, float fov, float yaw, float pitch, Node* parent);

	inline glm::mat4 GetLookAtMatrix() { return glm::lookAt(transform.position, transform.position + front, up); }
	inline glm::mat4 GetProjectionMatrix() { return this->projectionMatrixG; }
	inline float GetFOV() { return this->fov; }
	inline float GetYaw() { return this->yaw; }
	inline float GetPitch() { return this->pitch; }
	inline float GetNear() { return this->near; }
	inline float GetFar() { return this->far; }
	inline glm::vec3 GetPosition() { return this->position; }

	void SetFOV(float fov);
	void SetAspectRatio(float aspect);
	void SetNear(float near);
	void SetFar(float far);

	//Input
	void ProcessKeyboard(MovementDir direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset);
	void ProcessMouseScroll(float yOffset);

	void UpdateYaw(float yaw);
	void UpdatePitch(float pitch);

	void PMRow0(glm::vec2 row);
	void PMRow1(glm::vec2 row);
	void PMRow2(glm::vec2 row);

	void UpdatePM();
	void UpdateL();
private:

	//Camera attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw; //Rotation based on Y axis(left and right). -90 means pointing to the negative Z axis, that for us means forward.
	float pitch; //Rotation based on X axis(up and down)
	float speed;
	float sensitivity;
	float fov;

	float near;
	float far;
	float aspectRatio;

	glm::mat4 projectionMatrixAux = glm::mat4{ 1.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 1.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 1.0f,
											0.0f, 0.0f, -1.0f, 0.0f };
	glm::mat4 projectionMatrixL;
	glm::mat4 projectionMatrixG;

	void CalculateVectors();
};

#endif 
