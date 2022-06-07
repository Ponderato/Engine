#pragma once

#ifndef HIERARCHYPANEL_H
#define HIERARCHYPANEL_H

#include "Panel.h"
#include "Node.h"

class HierarchyPanel : public Panel
{
public:

	HierarchyPanel() = default;
	HierarchyPanel(const Context& context);

	void OnImGuiRender() override;
private:

	Node selectedNode;

	void DrawParentNodes(std::vector<Node*> nodes);
	void DrawChildNodes(std::vector<Node*> nodes);
	void DrawNode(Node* node);
};

#endif