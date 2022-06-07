#include "HierarchyPanel.h"


HierarchyPanel::HierarchyPanel(const Context& context) : Panel(context) {
}

void HierarchyPanel::OnImGuiRender() {

	ImGui::Begin("Hierarchy");

	DrawParentNodes(context.models);
	
	ImGui::End();
}

void HierarchyPanel::DrawParentNodes(std::vector<Node*> nodes) {

	for each (Node* node in nodes) {
		if (node->parent == context.parentNode)
			DrawNode(node);
	}
}

void HierarchyPanel::DrawChildNodes(std::vector<Node*> nodes) {

	for each (Node * node in nodes) {
		DrawNode(node);
	}
}

void HierarchyPanel::DrawNode(Node* node) {

	auto& tag = node->tag; //We keep a reference so we can change the name later

	ImGuiTreeNodeFlags flags = ((selectedNode.ID == node->ID) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)node, flags, tag.c_str());

	if (ImGui::IsItemClicked()) {
		selectedNode = *node;
	}

	if (opened) {

		if (node->children.size() > 0)
			DrawChildNodes(node->children);

		ImGui::TreePop();
	}
}