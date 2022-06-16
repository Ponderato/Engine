#include "InspectorPanel.h"

#include <imgui_internal.h>
#include <gtc/type_ptr.hpp>

InspectorPanel::InspectorPanel(Context* context) : Panel(context) {
}

void InspectorPanel::OnImGuiRender() {

	ImGui::Begin("Inspector");

	if (selected)
		DrawComponents(selectedNode);

	ImGui::End();
}

void InspectorPanel::DrawVec3(const std::string& label, glm::vec3* values, float reset, float columnWidth) {

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.25f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	if (ImGui::Button("X", buttonSize))
		values->x = reset;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values->x, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	if (ImGui::Button("Y", buttonSize))
		values->y = reset;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values->y, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	if (ImGui::Button("Z", buttonSize))
		values->z = reset;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values->z, 0.1f);
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void InspectorPanel::DrawFloat(const std::string& label, float* value, ImVec4 color, float reset, float columnWidth) {

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, color.w + 0.1f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

	if (ImGui::Button(" ", buttonSize))
		*value = reset;

	ImGui::PopStyleColor(3);
	ImGui::SameLine();

	ImGui::DragFloat("##.", value, 0.1f);
}

void InspectorPanel::DrawTag(char buffer[]) {

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70);
	ImGui::Text("Tag");
	ImGui::NextColumn();

	ImGui::InputText("##Tag", buffer, sizeof(buffer));
		
	ImGui::Columns(1);
}

void InspectorPanel::DrawComponents(Node node) {

	//Get a reference to the given node so we can work with it.
	Node* NODE = 0;
	for(Node * auxNode : context->nodes) {
		if (auxNode->ID == node.ID)
			NODE = auxNode;
	}

	Camera* camera = dynamic_cast<Camera*>(NODE);

	//--------------------------TAG--------------------------------
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));						//Set buffer to 0
	strcpy_s(buffer, sizeof(buffer), NODE->tag.c_str());	//Copy the tag into the buffer to then pass it to the input text

	DrawTag(buffer);
	NODE->tag = std::string(buffer);

	//--------------------------TRANSFORM--------------------------------
	#pragma region TRANSFORM
	if (ImGui::CollapsingHeader("Transform")) {

		//--------------------------POSITION--------------------------------
		glm::vec3 pos = NODE->transform.position;
		DrawVec3("Position", &pos, 0.0f, 70.0f);
		NODE->Move(pos);

		//--------------------------SCALE--------------------------------
		if (!camera) {
			glm::vec3 scale = NODE->transform.scale;
			DrawVec3("Scale", &scale, 1.0f, 70.0f);
			NODE->Scale(scale);
		}

		//--------------------------ROTATION--------------------------------
		if (!camera) {
			glm::vec3 rotation = glm::vec3(NODE->transform.rotation.x, NODE->transform.rotation.y, NODE->transform.rotation.z);
			DrawVec3("Rotation", &rotation, 0.0f, 70.0f);
			rotation = ChekRotation(rotation);
			NODE->Rotate(glm::vec3(rotation.x, rotation.y, rotation.z));
		}

		//--------------------------CAMERA STUFF--------------------------------
		if (camera) {
			float fov = camera->fov;
			DrawFloat("FOV", &fov, ImVec4(0.8f, 0.8f, 0.0f, 1.0f), 45.0f, 70.0f);
			camera->SetFOV(fov);
		}

	}
	#pragma endregion

	//--------------------------MATRIX--------------------------------
	#pragma region MATRIX
	if (!camera) {
		if (ImGui::CollapsingHeader("Local Matrix")) {

			glm::vec4 row = glm::vec4(NODE->transform.localModel[0].x, NODE->transform.localModel[1].x, NODE->transform.localModel[2].x, NODE->transform.localModel[3].x);
			if (ImGui::DragFloat4(" ", glm::value_ptr(row), 0.1f)) {
				NODE->Move(glm::vec3(row.w, NODE->transform.localModel[3].y, NODE->transform.localModel[3].z));
				NODE->Scale(glm::vec3(row.x, NODE->transform.localModel[1].y, NODE->transform.localModel[2].z));
			}

			row = glm::vec4(NODE->transform.localModel[0].y, NODE->transform.localModel[1].y, NODE->transform.localModel[2].y, NODE->transform.localModel[3].y);
			if (ImGui::DragFloat4("  ", glm::value_ptr(row), 0.1f)) {
				NODE->Move(glm::vec3(NODE->transform.localModel[3].x, row.w, NODE->transform.localModel[3].z));
				NODE->Scale(glm::vec3(NODE->transform.localModel[0].x, row.y, NODE->transform.localModel[2].z));
			}

			row = glm::vec4(NODE->transform.localModel[0].z, NODE->transform.localModel[1].z, NODE->transform.localModel[2].z, NODE->transform.localModel[3].z);
			if (ImGui::DragFloat4("   ", glm::value_ptr(row), 0.1f)) {
				NODE->Move(glm::vec3(NODE->transform.localModel[3].x, NODE->transform.localModel[3].y, row.w));
				NODE->Scale(glm::vec3(NODE->transform.localModel[0].x, NODE->transform.localModel[1].y, row.z));
			}

			row = glm::vec4(NODE->transform.localModel[0].w, NODE->transform.localModel[1].w, NODE->transform.localModel[2].w, NODE->transform.localModel[3].w);
			if (ImGui::DragFloat4("    ", glm::value_ptr(row), 0.1f)) {
			}
		}
	}

	#pragma endregion
}

glm::vec3 InspectorPanel::ChekRotation(glm::vec3 rotation) {

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

	return rotation;
}
