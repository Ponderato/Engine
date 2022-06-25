#include "RenderPanel.h"

RenderPanel::RenderPanel(Context* context) : Panel(context) {
}

void RenderPanel::OnImGuiRender() {

	ImGui::Begin("RenderView");
	//ImVec2 size = ImGui::GetWindowSize();
	ImVec2 size = ImGui::GetContentRegionAvail();

	//Maintain Aspect Ratio 
	context->camera.SetAspectRatio((float)size.x / size.y);

	//Final two vec2 to invert the image. This is needed because OpenGL and ImGui screen coordinates are the opposite.
	ImGui::Image((void*)(intptr_t)renderImg, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}
