#include "RenderPanel.h"

RenderPanel::RenderPanel(Context* context) : Panel(context) {
}

void RenderPanel::OnImGuiRender() {

	ImGui::Begin("RenderView");

	if (ImGui::IsWindowFocused())
		this->focused = true;
	else
		this->focused = false;

	//ImVec2 size = ImGui::GetWindowSize();
	ImVec2 size = ImGui::GetContentRegionAvail();

	//Maintain Aspect Ratio 
	context->GetActiveCamera()->SetAspectRatio((float)size.x / size.y);
	//context->camera->SetAspectRatio(context->camera->aspectRatio);

	//Final two vec2 to invert the image. This is needed because OpenGL and ImGui screen coordinates are the opposite.
	ImGui::Image((void*)(intptr_t)renderImg, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));


	DrawData();

	ImGui::End();
}

void RenderPanel::DrawData() {

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2, 0.6, 0.8, 1));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 0.8));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.8, 0.8, 1));

	ImGui::SetCursorPosY(30);

	//Active pipeline
	ImGui::SetCursorPosX(30);

	if (ImGui::Button("Pipeline", ImVec2(70, 20))) {
		if (context->GetActivePipe()->Name == "Deferred") {
			context->SetActivePipeline(context->pipelines.at(1));
		}
		else {
			context->SetActivePipeline(context->pipelines.at(0));
		}
	}
	ImGui::SameLine();
	ImGui::Text(":");
	ImGui::SameLine();
	ImGui::Text(context->GetActivePipe()->Name.c_str());

	ImGui::SameLine();

	//FPS
	ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 200);

	ImGui::Text(std::to_string(fps).c_str());
	ImGui::SameLine();
	ImGui::Text("FPS / ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(ms).c_str());
	ImGui::SameLine();
	ImGui::Text("Ms");

	ImGui::PopStyleColor(3);
}
