#include "InspectorPanel.h"

InspectorPanel::InspectorPanel(const Context& context) : Panel(context) {
}

void InspectorPanel::OnImGuiRender() {

	ImGui::Begin("Inspector");

	if (selected)
		DrawComponents(selectedNode);

	ImGui::End();
}

void InspectorPanel::DrawComponents(Node node) {

	//TAG COMPONENT

	//auto& tag = node.tag; //We keep a reference so we can change the name later
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));						//Set buffer to 0
	strcpy_s(buffer, sizeof(buffer), node.tag.c_str());		//Copy the tag into the buffer to then pass it to the input text

	//TODO -> NOT EFFICIENT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
	if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
		for each (Node* auxNode in context.models) {
			if (auxNode->ID == node.ID)
				auxNode->tag = std::string(buffer);
		}
	}

}
