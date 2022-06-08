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

	inline Node GetSelectedNode() { return this->selectedNode; }
	inline bool isSelected() { return this->selected; }

	void OnImGuiRender() override;

	
private:

	Node selectedNode;
	bool selected = false;

	void DrawParentNodes(std::vector<Node*> nodes);
	void DrawChildNodes(std::vector<Node*> nodes);
	void DrawNode(Node* node);

	void DrawComponents(Node node);
};

#endif