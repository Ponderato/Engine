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
	void DrawVec3(const std::string& label, glm::vec3* values, float reset = 0.0f, float columnWidth = 70.0f);
	void DrawFloat(const std::string& label, float* values, ImVec4 color, float reset = 0.0f, float columnWidth = 70.0f);
	void DrawTag(char buffer[]);

	glm::vec3 ChekRotation(glm::vec3 rotation);
};

#endif