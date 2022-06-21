#pragma once

#ifndef PARENTPANEL_H
#define PARENTPANEL_H

#include "Panel.h"

class ParentPanel : public Panel
{
public:

	bool close = false;

	ParentPanel() = default;
	ParentPanel(Context* context);

	void OnImGuiRender() override;

	inline void SetNodeToParent(Node* node) { this->nodeToParent = node; };

private:

	Node selectedNode;
	Node* nodeToParent;

	void DrawParentNodes(std::vector<Node*> nodes);
	void DrawChildNodes(std::vector<Node*> nodes);
	void DrawNode(Node* node);

	void ParentNode();
};

#endif