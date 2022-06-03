#include "HierarchyPanel.h"

HierarchyPanel::HierarchyPanel(const Context& context) : Panel(context) {
}

void HierarchyPanel::OnImGuiRender() {

	ImGui::Begin("Hierarchy");
	ImGui::End();
}