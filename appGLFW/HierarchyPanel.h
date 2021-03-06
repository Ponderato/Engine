#pragma once

#ifndef HIERARCHYPANEL_H
#define HIERARCHYPANEL_H

#include "Panel.h"
#include "AddPanel.h"
#include "Node.h"

class HierarchyPanel : public Panel
{
public:

	HierarchyPanel() = default;
	HierarchyPanel(Context* context);

	inline Node GetSelectedNode() { return this->selectedNode; }
	inline void SetSelected(bool selected) { this->selected = selected; }
	inline bool isSelected() { return this->selected; }

	void OnImGuiRender() override;

	
private:

	AddPanel* addPanel = new AddPanel();

	Node selectedNode;
	bool selected = false;

	void DrawParentNodes(std::vector<Node*> nodes);
	void DrawChildNodes(std::vector<Node*> nodes);
	void DrawNode(Node* node);

	void DrawComponents(Node node);
};

#endif