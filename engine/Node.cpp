#include "Node.h"

void Node::Update() {

	if (parent == nullptr) {
		transform.globalModel = transform.localModel;
	}else {
		transform.globalModel = parent->transform.globalModel * transform.localModel;
	}

	for (Node* children : children) {
		children->Update();
	}
}

void Node::Parent(Node* parent) {

	if (parent == nullptr) {

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

		this->parent = parent;
		this->parent->children.push_back(this);

		transform.localModel = glm::inverse(parent->transform.globalModel) * transform.globalModel;
	}

}

//Checks if the given Node is a child of this node
bool Node::IsChild(Node* node) {

	if (node->parent == nullptr) return false;

	if (node->parent == this) return true;

	return this->IsChild(node->parent);
}

void Node::Render() {

}