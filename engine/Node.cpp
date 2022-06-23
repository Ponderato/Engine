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

	//Avoid cycles.
	//It can not be its own parent
	if (parent == this) return;

	//Avoid cyclic parents
	if (IsChild(parent)) return;

	transform.globalModel = glm::translate(transform.globalModel, transform.position);
	transform.globalModel = glm::rotate(transform.globalModel, glm::radians(360.0f), glm::vec3(transform.rotation.x, transform.rotation.y, transform.rotation.z));
	transform.globalModel = glm::scale(transform.globalModel, transform.scale);

	transform.localModel = glm::inverse(parent->transform.globalModel) * transform.globalModel;

	this->parent = parent;
	this->parent->children.push_back(this);
}

void Node::UnParent() {

	if (this->parent->parent) {

		int index = 0;
		for (Node* node : this->parent->children) {
			if (this->ID == node->ID) {
				this->parent->children.erase(this->parent->children.begin() + index);
			}
			index++;
		}

		NewParent(this->parent->parent);
	}
}

void Node::NewParent(Node* parent) {
	//Avoid cycles.
	//It can not be its own parent
	if (parent == this) return;

	//Avoid cyclic parents
	if (IsChild(parent)) return;

	glm::mat4 position = glm::translate(glm::mat4(1.0f), transform.position);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x), glm::vec3(transform.rotation.x, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0, transform.rotation.y, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0, 0, transform.rotation.z));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.scale);

	transform.localModel = position * rotation * scale;

	dirty = true;

	this->parent = parent;
	this->parent->children.push_back(this);
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

void Node::Move(const glm::vec3 newPos) {

	transform.position = newPos;

	transform.localModel[3].x = newPos.x;
	transform.localModel[3].y = newPos.y;
	transform.localModel[3].z = newPos.z;
	dirty = true;
}

void Node::Scale(const glm::vec3 scaleVector) {

	transform.scale = scaleVector;

	//Since i can do this like below instead of like in rotate method, i do it thta way cause is more efficient. Same happens with Move method
	transform.localModel[0].x = scaleVector.x;
	transform.localModel[1].y = scaleVector.y;
	transform.localModel[2].z = scaleVector.z;
	dirty = true;
}

void Node::Rotate(const glm::vec3 axis) {

	transform.rotation.x = axis.x;
	transform.rotation.y = axis.y;
	transform.rotation.z = axis.z;

	glm::mat4 position = glm::translate(glm::mat4(1.0f), transform.position);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(axis.x), glm::vec3(axis.x, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(axis.y), glm::vec3(0, axis.y, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(axis.z), glm::vec3(0, 0, axis.z));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.scale);

	transform.localModel = position * rotation * scale;
	dirty = true;
}


//Needed to be able to dynamically cast in the steps
void Node::Aux() {}