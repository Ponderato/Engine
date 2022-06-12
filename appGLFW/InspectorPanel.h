#pragma once

#ifndef INSPECTORPANEL_H
#define INSPECTORPANEL_H

#include "Panel.h"

class InspectorPanel : public Panel
{
public:

	InspectorPanel() = default;
	InspectorPanel(Context* context);

	inline void SetSelectedNode(Node node) { this->selectedNode = node; }
	inline void SetSelected(bool selected) { this->selected = selected; }

	void OnImGuiRender() override;


private:

	Node selectedNode;
	bool selected = false;

	void DrawComponents(Node node);
	void DrawVec3(const std::string& label, glm::vec3* values, float reset = 0.0f, float columnWidth = 100.0f);
	glm::vec3 ChekRotation(glm::vec3 rotation);
};

#endif