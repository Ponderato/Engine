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
	Camera(glm::vec3 position, glm::vec3 worldUp, float aspect, float speed, float sensitivity, float fov, float yaw, float pitch, Node* parent);

	inline glm::mat4 GetLookAtMatrix() { return glm::lookAt(transform.position, transform.position + front, up); }
	inline glm::mat4 GetProjectionMatrix() { return this->projectionMatrixG; }
	inline float GetFOV() { return this->fov; }
	inline float GetYaw() { return this->yaw; }
	inline float GetPitch() { return this->pitch; }
	inline float GetNear() { return this->near; }
	inline float GetFar() { return this->far; }
	inline float GetAR() { return this->aspectRatio; }
	inline glm::vec3 GetPosition() { return this->position; }
	inline bool GetDrawFrustum() { return this->drawFrustum; }
	inline glm::vec3 GetFrustumColor() { return this->frustumColor; }

	inline void SetDrawFrustum(bool draw) { this->drawFrustum = draw; }
	inline void SetFrustumColor(glm::vec3 color) { this->frustumColor = color; }

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

	void UpdatePM();
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

	bool drawFrustum = true;
	glm::vec3 frustumColor = glm::vec3(0.607f, 0.137f, 0.713f);

	glm::mat4 projectionMatrixAux = glm::mat4{ 1.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 1.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 1.0f,
											0.0f, 0.0f, -1.0f, 0.0f };
	glm::mat4 projectionMatrixL;
	glm::mat4 projectionMatrixG;

	void CalculateVectors();
};

#endif 
