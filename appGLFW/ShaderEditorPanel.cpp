#include "ShaderEditorPanel.h"

ShaderEditorPanel::ShaderEditorPanel(Context* context) : Panel(context) {}

void ShaderEditorPanel::OnImGuiRender() {

	ImGui::Begin("Shader Editor");
	
	editor.Render("Editor");

	ImGui::End();
}

