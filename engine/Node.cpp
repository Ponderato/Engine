#include "Node.h"
#include <chrono>

void Node::UpdateMatrix() {

	if (parent == nullptr) {
		transform.globalModel = transform.localModel;
	}else {
		transform.globalModel = parent->transform.globalModel * transform.localModel;
	}

	for (Node* children : children) {
		children->UpdateMatrix();
	}
}

void Node::Parent(Node* parent) {

	if (parent == nullptr) {

		//TODO -> NOSOS SIN PADRE DEBEN SER HIJOS DEL NODO RAÍZ DE LA ESCENA
		this->parent = nullptr;

		//Matrices calculations when there is no parent
		transform.localModel = glm::translate(transform.localModel, transform.position);
		transform.localModel = glm::rotate(transform.localModel, glm::radians(transform.rotation.w), glm::vec3(transform.rotation.x, transform.rotation.y, transform.rotation.z));
		transform.localModel = glm::scale(transform.localModel, transform.scale);
				 
		transform.globalModel = transform.localModel;
	}
	else {
		//Avoid cycles.
		//It can not be its own parent
		if (parent == this) return;

		//Avoid cyclic parents
		if (IsChild(parent)) return;

		transform.globalModel = glm::translate(transform.globalModel, transform.position);
		transform.globalModel = glm::rotate(transform.globalModel, glm::radians(transform.rotation.w), glm::vec3(transform.rotation.x, transform.rotation.y, transform.rotation.z));
		transform.globalModel = glm::scale(transform.globalModel, transform.scale);

		transform.localModel = glm::inverse(parent->transform.globalModel) * transform.globalModel;

		this->parent = parent;
		this->parent->children.push_back(this);
	}
}

//Checks if the given Node is a child of this node
bool Node::IsChild(Node* node) {

	if (node->parent == nullptr) return false;

	if (node->parent == this) return true;

	return this->IsChild(node->parent);
}

void Node::Update() {

	if (dirty) {
		//Update nodes' matrices
		UpdateMatrix();
		dirty = false;
	}
}

void Node::Move(const glm::vec3 newPos, const float speed) {

	transform.localModel = glm::translate(transform.localModel, newPos * speed);
	dirty = true;
}

void Node::Scale(const glm::vec3 scaleVector) {

	transform.localModel = glm::scale(transform.localModel, scaleVector);
	dirty = true;
}

void Node::Scale(const float scaleFactor) {

	transform.localModel = glm::scale(transform.localModel, glm::vec3(scaleFactor));
	dirty = true;
}

void Node::Rotate(const glm::vec3 axis, const float angle, const float speed) {

	transform.localModel = glm::rotate(transform.localModel, speed * glm::radians( angle), axis);
	dirty = true;
}