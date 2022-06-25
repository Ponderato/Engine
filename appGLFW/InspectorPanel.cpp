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
	LightCube* light = dynamic_cast<LightCube*>(NODE);

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

	//--------------------------INTENSITY--------------------------------
	if (light) {
		float intensity = light->intensity;
		DrawFloat("Intensity", &intensity, ImVec4(1.0f, 0.0f, 1.0f, 1.0f), 1.0f, 80.0f);
		light->SetIntensity(intensity);

		glm::vec3 color = light->color;
		DrawVec3RGB("Color", &color, 1.0f, 80.0f);
		light->SetColor(color);
	}

	//--------------------------CAMERA STUFF--------------------------------
	if (camera) {
		float fov = camera->fov;
		DrawFloat("FOV", &fov, ImVec4(0.8f, 0.8f, 0.0f, 1.0f), 45.0f, 80.0f);
		camera->SetFOV(fov);

		//ImGui::NextLine();

		float yaw = camera->yaw;
		DrawFloat("YAW (Y)", &yaw, ImVec4(1.0f, 0.5f, 0.0f, 1.0f), -90.0f, 80.0f);
		camera->UpdateYaw(yaw);

		float pitch = camera->pitch;
		DrawFloat("PITCH (X)", &pitch, ImVec4(0.4f, 0.2f, 0.05f, 1.0f), 0.0f, 80.0f);
		camera->UpdatePitch(pitch);

		float near = camera->near;
		DrawFloat("NEAR", &near, ImVec4(0.9f, 0.1f, 0.1f, 1.0f), 0.1f, 80.0f);
		camera->SetNear(near);

		float far = camera->far;
		DrawFloat("FAR", &far, ImVec4(0.7f, 0.05f, 0.05f, 1.0f), 100.0f, 80.0f);
		camera->SetFar(far);

	}

	//--------------------------TRANSFORM--------------------------------
	#pragma region TRANSFORM
	if (ImGui::CollapsingHeader("Transform")) {

		//--------------------------POSITION--------------------------------
		glm::vec3 pos = NODE->transform.position;
		DrawVec3("Position", &pos, 0.0f, 80.0f);
		NODE->Move(pos);

		//--------------------------SCALE--------------------------------
		if (!camera) {

			glm::vec3 scale = NODE->transform.scale;
			DrawVec3("Scale", &scale, 1.0f, 80.0f);
			NODE->Scale(scale);

			//--------------------------ROTATION--------------------------------
			glm::vec3 rotation = glm::vec3(NODE->transform.rotation.x, NODE->transform.rotation.y, NODE->transform.rotation.z);
			DrawVec3("Rotation", &rotation, 0.0f, 80.0f);
			rotation = ChekRotation(rotation);
			NODE->Rotate(glm::vec3(rotation.x, rotation.y, rotation.z));
		}

	}
	#pragma endregion

	//--------------------------MATRIX--------------------------------
	#pragma region MATRIX
	if (!camera)
		DrawMatrix("Local Matrix", NODE);
	else
		DrawMatrix("Projection Matrix", NODE);

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

void InspectorPanel::DrawMatrix(std::string label, Node* node) {

	Camera* camera = dynamic_cast<Camera*>(node);
	glm::mat4 matrix;

	if (camera)
		matrix = camera->projectionMatrix;
	else
		matrix = node->GetLocalMatrix();

	if (ImGui::CollapsingHeader(label.c_str())) {

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
		ImGui::Text("       [0]     [1]     [2]     [3]  ");
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
		ImGui::Text("[x]");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		glm::vec4 row = glm::vec4(matrix[0].x, matrix[1].x, matrix[2].x, matrix[3].x);
		if (ImGui::DragFloat4(" ", glm::value_ptr(row), 0.1f)) {
			if (camera) {
				camera->PMRow1(glm::vec2(row.x, row.z));
			}
			else {
				node->Move(glm::vec3(row.w, matrix[3].y, matrix[3].z));
				node->Scale(glm::vec3(row.x, matrix[1].y, matrix[2].z));
			}

		}

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
		ImGui::Text("[y]");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		row = glm::vec4(matrix[0].y, matrix[1].y, matrix[2].y, matrix[3].y);
		if (ImGui::DragFloat4("  ", glm::value_ptr(row), 0.1f)) {
			if (camera) {

			}
			else {
				node->Move(glm::vec3(matrix[3].x, row.w, matrix[3].z));
				node->Scale(glm::vec3(matrix[0].x, row.y, matrix[2].z));
			}
		}

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
		ImGui::Text("[z]");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		row = glm::vec4(matrix[0].z, matrix[1].z, matrix[2].z, matrix[3].z);
		if (ImGui::DragFloat4("   ", glm::value_ptr(row), 0.1f)) {
			if (camera) {

			}
			else {
				node->Move(glm::vec3(matrix[3].x, matrix[3].y, row.w));
				node->Scale(glm::vec3(matrix[0].x, matrix[1].y, row.z));
			}
		}

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
		ImGui::Text("[w]");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		row = glm::vec4(matrix[0].w, matrix[1].w, matrix[2].w, matrix[3].w);
		if (ImGui::DragFloat4("    ", glm::value_ptr(row), 0.1f)) {
		}
	}
}