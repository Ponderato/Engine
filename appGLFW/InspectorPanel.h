#pragma once

#ifndef INSPECTORPANEL_H
#define INSPECTORPANEL_H

#include "Panel.h"
#include "ParentPanel.h"

class InspectorPanel : public Panel
{
public:

	InspectorPanel() = default;
	InspectorPanel(Context* context);

	inline void SetSelectedNode(Node node) { this->selectedNode = node; }
	inline void SetSelected(bool selected) { this->selected = selected; }
	inline bool isSelected() { return this->selected; }

	void OnImGuiRender() override;

private:

	ParentPanel* parentPanel = new ParentPanel();

	Node selectedNode;
	bool selected = false;

	void DrawComponents(Node node);
	void DrawMatrix(std::string label, Node* node);

	glm::vec3 ChekRotation(glm::vec3 rotation);
};

#endif