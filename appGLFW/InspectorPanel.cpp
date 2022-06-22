#include "InspectorPanel.h"

InspectorPanel::InspectorPanel(Context* context) : Panel(context) {}

void InspectorPanel::OnImGuiRender() {

	ImGui::Begin("Inspector");

	if (selected)
		DrawComponents(selectedNode);

	parentPanel->OnImGuiRender();

	ImGui::End();
}

void InspectorPanel::DrawComponents(Node node) {

	//Get a reference to the given node so we can work with it.
	Node* NODE = new Node();
	
	for (Node* auxNode : context->nodes) {
		if (auxNode->ID == node.ID)
			NODE = auxNode;
	}

	Camera* camera = dynamic_cast<Camera*>(NODE);

	//--------------------------DELETE BUTTON--------------------------------
	if (!camera) {
		if (ImGui::Button("Remove", ImVec2(50, 20))) {
			context->DeleteNode(NODE);
			this->selected = false;
		}
	


	//--------------------------PARENTING BUTTON--------------------------------
	
	
		ImGui::SameLine();
		if (ImGui::Button("Parent", ImVec2(50, 20))) {
			parentPanel->close = true;
			parentPanel->SetContext(this->context);
			parentPanel->SetNodeToParent(NODE);
		}
	}

	//--------------------------UNPARENTING BUTTON--------------------------------
	if (NODE->parent != this->context->parentNode) {
		ImGui::SameLine();
		if (ImGui::Button("Unparent", ImVec2(70, 20))) {
			NODE->UnParent();
		}
	}

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


			//--------------------------ROTATION--------------------------------

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

			//ImGui::NextLine();

			float yaw = camera->yaw;
			DrawFloat("YAW (Y)", &yaw, ImVec4(0.2f, 0.7f, 0.2f, 1.0f), -90.0f, 70.0f);
			camera->UpdateYaw(yaw);

			float pitch = camera->pitch;
			DrawFloat("PITCH (X)", &pitch, ImVec4(0.8f, 0.1f, 0.15f, 1.0f), 0.0f, 70.0f);
			camera->UpdatePitch(pitch);

			//if (ImGui::Button("Mouse", ImVec2(50, 20))) {
			//	if(!camera->moveMouse)
			//		camera->moveMouse = true;
			//	else
			//		camera->moveMouse = false;
			//}

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
