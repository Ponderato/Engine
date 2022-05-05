#pragma once

#ifndef NODE_H
#define NODE_H

#include <glm.hpp>
#include <vector>

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	glm::mat4 localMat;
	glm::mat4 globalMat;
};

class Node
{
public:
	
	Transform transform;
	Node* parent;
	std::vector<Node*> children;

	void Update(Node* parent);

	void Render();
};

#endif 