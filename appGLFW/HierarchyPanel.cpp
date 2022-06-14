#include "HierarchyPanel.h"


HierarchyPanel::HierarchyPanel(Context* context) : Panel(context) {
}

void HierarchyPanel::OnImGuiRender() {

	ImGui::Begin("Hierarchy");

	DrawParentNodes(context->nodes);

	//if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	//	selectedNode = {};
	
	ImGui::End();
}

void HierarchyPanel::DrawParentNodes(std::vector<Node*> nodes) {

	for(Node* node : nodes) {
		if (node->parent == context->parentNode)
			DrawNode(node);
	}
}

void HierarchyPanel::DrawChildNodes(std::vector<Node*> nodes) {

	for(Node * node : nodes) {
		DrawNode(node);
	}
}

void HierarchyPanel::DrawNode(Node* node) {

	ImGuiTreeNodeFlags flags = ((selectedNode.ID == node->ID) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)node, flags, node->tag.c_str());

	if (ImGui::IsItemClicked()) {
		selectedNode = *node;
		selected = true;
	}
	
	if (opened) {

		if (node->children.size() > 0)
			DrawChildNodes(node->children);

		ImGui::TreePop();
	}
}
