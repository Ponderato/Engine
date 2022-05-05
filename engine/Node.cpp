#include "Node.h"

void Node::Update(Node* parent) {

	if (parent == nullptr) {
		transform.globalMat = transform.localMat;
	}else {
		transform.globalMat = parent->transform.globalMat * transform.localMat;
	}

	for each(Node* children in children) {
		children->Update(this);
	}
}

void Node::Render() {

}