#include "InspectorPanel.h"

InspectorPanel::InspectorPanel(const Context& context) : Panel(context) {
}

void InspectorPanel::OnImGuiRender() {

	ImGui::Begin("Inspector");
	ImGui::End();
}