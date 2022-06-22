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

	bool moveMouse = false;

	glm::mat4 projectionMatrix;

	Camera() = default;
	Camera(glm::vec3 position, glm::vec3 worldUp, float speed, float sensitivity, float fov, float yaw, float pitch, Node* parent);

	inline glm::mat4 GetLookAtMatrix() { return glm::lookAt(transform.position, transform.position + front, up); }
	inline glm::mat4 GetProjectionMatrix() { return glm::perspective(glm::radians(fov), aspectRatio, near, far); }
	inline void SetFOV(float fov) { this->fov = fov; }
	inline void SetAspectRatio(float aspect) { this->aspectRatio = aspect; }
	inline void SetNear(float near) { this->near = near; }
	inline void SetFar(float far) { this->far = far; }

	//Input
	void ProcessKeyboard(MovementDir direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset);
	void ProcessMouseScroll(float yOffset);

	void UpdateYaw(float yaw);
	void UpdatePitch(float pitch);


	void Resize();
private:

	void CalculateVectors();
};

#endif 
