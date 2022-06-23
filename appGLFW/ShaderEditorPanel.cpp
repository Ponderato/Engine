#include "ShaderEditorPanel.h"

//Open file
#include "windows.h"
#include "commdlg.h"
#include "glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"

#include <sstream>

ShaderEditorPanel::ShaderEditorPanel(Context* context) : Panel(context) {}

void ShaderEditorPanel::OnImGuiRender() {

	ImGui::Begin("Shader Editor");
	
	SelectFile();

	editor.Render("Editor");

	//std::string aux = editor.GetText();
	//
	//std::cout << aux << std::endl;

	ImGui::End();
}

void ShaderEditorPanel::ConfigureEditor() {

	//Style
	editor.SetShowWhitespaces(false);
	editor.SetPalette(TextEditor::GetDarkPalette());

}

void ShaderEditorPanel::SelectFile() {

	if (ImGui::Button("Select File", ImVec2(90, 20))) {

		filePath = OpenFile("Shaders (*.glsl)\0*.glsl\0");
		fileName = GetFileName(filePath);

		std::string content = GetFileContent(filePath);
		editor.SetText(content);

	}

	ImGui::SameLine();
	//ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
	TextCentered(fileName.c_str());
	ImGui::PopStyleColor();
}

void ShaderEditorPanel::TextCentered(std::string text) {
	float win_width = ImGui::GetWindowSize().x + 90; //+ 90 from the select file button
	float text_width = ImGui::CalcTextSize(text.c_str()).x;

	// calculate the indentation that centers the text on one line, relative
	// to window left, regardless of the `ImGuiStyleVar_WindowPadding` value
	float text_indentation = (win_width - text_width) * 0.5f;

	// if text is too long to be drawn on one line, `text_indentation` can
	// become too small or even negative, so we check a minimum indentation
	float min_indentation = 20.0f;
	if (text_indentation <= min_indentation) {
		text_indentation = min_indentation;
	}

	ImGui::SameLine(text_indentation);
	ImGui::PushTextWrapPos(win_width - text_indentation);
	ImGui::TextWrapped(text.c_str(), 15);
	ImGui::PopTextWrapPos();
}

std::string ShaderEditorPanel::OpenFile(const char* filter) {

	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE) {
		return ofn.lpstrFile;
	}

	return fileName;
}

std::string ShaderEditorPanel::GetFileName(std::string path) {

	std::string name;
	std::stringstream s_stream(path);

	while (s_stream.good()) {
		std::string substr;
		std::getline(s_stream, substr, '\\');
		name = substr;
	}

	return name;

}

std::string ShaderEditorPanel::GetFileContent(std::string path) {

	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}