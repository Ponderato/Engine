#pragma once

#ifndef NODE_H
#define NODE_H

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <vector>

struct Transform 
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec4 rotation = glm::vec4(360.0f); //vec3 for axis and 4th element for degrees
	//glm::quat rotation; //TODO QUATERNIONS

	glm::mat4 localModel = glm::mat4(1.0f);
	glm::mat4 globalModel = glm::mat4(1.0f);
};


class Node
{
public:
	
	Transform transform;

	Node* parent = nullptr;
	std::vector<Node*> children;

	bool renderable = false;
	bool dirty = false;	

	void Parent(Node* parent);
	bool IsChild(Node* node);

	void Move(const glm::vec3 newPos);
	void Scale(const glm::vec3 scaleVector);
	void Scale(const float scaleFactor);
	void Rotate(const float angle);
	void Rotate(const glm::vec3 axis, const float angle);

	void Update();
private:

	void UpdateMatrix();
};

#endif 