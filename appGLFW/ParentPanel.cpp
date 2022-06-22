#include "ParentPanel.h"

ParentPanel::ParentPanel(Context* context) : Panel(context) {}

void ParentPanel::OnImGuiRender() {

    if (close) {

        ImGui::SetNextWindowSize(ImVec2(300, 600));

        if (!ImGui::Begin("Parent Node", &close))
        {
            close = false;
            ImGui::End();
        }
        else
        {
			DrawParentNodes(context->nodes);
			if (ImGui::Button("Parent", ImVec2(50, 20))) {
				ParentNode();
				close = false;
			}
			
            ImGui::End();
        }
    }

}

void ParentPanel::DrawParentNodes(std::vector<Node*> nodes) {

	for (Node* node : nodes) {

		Camera* camera = dynamic_cast<Camera*>(node);
		
		if (!camera && node->parent == context->parentNode)
			DrawNode(node);
	}
}

void ParentPanel::DrawChildNodes(std::vector<Node*> nodes) {

	for (Node* node : nodes) {
		DrawNode(node);
	}
}

void ParentPanel::DrawNode(Node* node) {

	ImGuiTreeNodeFlags flags = ((selectedNode.ID == node->ID) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)node, flags, node->tag.c_str());

	if (ImGui::IsItemClicked()) {
		selectedNode = *node;
	}

	if (opened) {

		if (node->children.size() > 0) {
			DrawChildNodes(node->children);
		}

		ImGui::TreePop();
	}
}

void ParentPanel::ParentNode() {

	for (Node* node : context->nodes) {
		if (selectedNode.ID == node->ID) {
			nodeToParent->NewParent(node);
		}
	}


}