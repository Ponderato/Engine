#include "InspectorPanel.h"
#include "gtc/type_ptr.hpp"

InspectorPanel::InspectorPanel(Context* context) : Panel(context) {
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
	for each (Node* auxNode in context->models) {
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

	//--------------------------TRANSFORM--------------------------------
	if (ImGui::CollapsingHeader("Transform")) {
		//--------------------------POSITION--------------------------------
		glm::vec3 pos = NODE->transform.position;
		if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1f)) {
			NODE->Move(pos);
		}

		//--------------------------SCALE--------------------------------
		glm::vec3 scale = NODE->transform.scale;
		if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f)) {
			NODE->Scale(scale);
		}

		//--------------------------ROTATION--------------------------------
		glm::vec3 rotation = glm::vec3(NODE->transform.rotation.x, NODE->transform.rotation.y, NODE->transform.rotation.z);
		float degrees = NODE->transform.rotation.w;

		if (ImGui::DragFloat3("Axis", glm::value_ptr(rotation), 0.1f)) {

			if (rotation.x >= 361.0f) 
				rotation.x = 1.0f;

			if (rotation.x < 0.01f)
				rotation.x = 360.0f;

			if (rotation.y >= 361.0f)
				rotation.y = 1.0f;
						 
			if (rotation.y < 0.01f)
				rotation.y = 360.0f;

			if (rotation.z >= 361.0f)
				rotation.z = 1.0f;
						 
			if (rotation.z < 0.01f)
				rotation.z = 360.0f;

			NODE->transform.rotation.x = rotation.x;
			NODE->transform.rotation.y = rotation.y;
			NODE->transform.rotation.z = rotation.z;
			//NODE->Rotate(glm::vec3(rotation.x, rotation.y, rotation.z), degrees);
		}

		if (ImGui::DragFloat("Degrees", &degrees, 1.0f)) {

			if (degrees >= 361.0f)
				degrees = 1.0f;

			if (degrees < 0.01f)
				degrees = 360.0f;

			NODE->Rotate(glm::vec3(rotation.x, rotation.y, rotation.z), degrees);
			//NODE->transform.rotation.w = degrees;
		}

	}





}
