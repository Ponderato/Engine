#include "InspectorPanel.h"
#include "gtc/type_ptr.hpp"

InspectorPanel::InspectorPanel(const Context& context) : Panel(context) {
}

void InspectorPanel::OnImGuiRender() {

	ImGui::Begin("Inspector");

	if (selected)
		DrawComponents(selectedNode);

	ImGui::End();
}

void InspectorPanel::DrawComponents(Node node) {

	//Get a reference to the given node so we can work with it.
	Node* NODE = 0;
	for each (Node* auxNode in context.models) {
		if (auxNode->ID == node.ID)
			NODE = auxNode;
	}

	//--------------------------TAG--------------------------------
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));						//Set buffer to 0
	strcpy_s(buffer, sizeof(buffer), NODE->tag.c_str());		//Copy the tag into the buffer to then pass it to the input text

	if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
		NODE->tag = std::string(buffer);
	}

	//--------------------------POSITION--------------------------------
	if (ImGui::CollapsingHeader("Transform")) {
		glm::vec3 pos = NODE->transform.position;
		if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1f)) {
			NODE->Move(pos);
		}
	}



}
