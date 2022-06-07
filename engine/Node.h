#pragma once

#ifndef NODE_H
#define NODE_H

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <vector>
#include <string>

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

	Node() { static int id = 0; ID = id++; };
	
	Transform transform;

	Node* parent = nullptr;
	std::vector<Node*> children;

	std::string tag = "";
	int ID = 0;

	bool renderable = false;
	bool forward = false;
	bool dirty = false;	

	void Parent(Node* parent);
	bool IsChild(Node* node);

	void Move(const glm::vec3 newPos, const float speed);
	void Scale(const glm::vec3 scaleVector);
	void Scale(const float scaleFactor);
	void Rotate(const glm::vec3 axis, const float angle, const float speed);

	void Update();

	//Needed to be able to dynamically cast in the steps
	virtual void Aux();

	operator uint32_t() const { return (uint32_t)this; }
	//bool operator==(const Node& other) const { return this->ID == other.ID; }
	//bool operator!=(const Node& other) const { return !(*this == other); }

private:

	void UpdateMatrix();
};

#endif 