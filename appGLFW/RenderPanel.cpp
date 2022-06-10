#include "RenderPanel.h"

RenderPanel::RenderPanel(Context* context) : Panel(context) {
}

void RenderPanel::OnImGuiRender() {

	ImGui::Begin("RenderView");
	ImVec2 size = ImGui::GetWindowSize();
	//Final two vec2 to invert the image. This is needed because OpenGL and ImGui screen coordinates are the opposite.
	ImGui::Image((void*)(intptr_t)renderImg, ImVec2(size.x - 15, size.y - 35), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}
