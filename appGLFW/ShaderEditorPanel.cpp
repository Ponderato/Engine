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

	DrawFileName(fileName, ImVec4(1, 1, 0, 1));

	editor.Render("Editor", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 110));

	ImGui::Spacing();
	FileBtns();

	ImGui::End();
}

void ShaderEditorPanel::ConfigureEditor() {

	//Style
	editor.SetShowWhitespaces(false);
	editor.SetPalette(TextEditor::GetDarkPalette());
}

void ShaderEditorPanel::SelectFile() {

	if (ImGui::CollapsingHeader("Select File")) {

		int count = 0;

		for (Program program : context->programs) {

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowSize().x / 2 - program.vsName.length() * 8);

			if (ImGui::Button(program.vsName.c_str(), ImVec2(program.vsName.length() * 8, 20))) {
				filePath = program.vsPath;
				fileName = program.vsName;
				programNum = count;
			}

			//ImGui::Separator();
			ImGui::SameLine();

			if (ImGui::Button(program.fsName.c_str(), ImVec2(program.fsName.length() * 8, 20))) {
				filePath = program.fsPath;
				fileName = program.fsName;
				programNum = count;
			}

			std::string content = GetFileContent(filePath);
			editor.SetText(content);

			count++;
		}
	}

	if (ImGui::Button("Save", ImVec2(50, 20))) {

		if (fileName != "No File Opened") {

			std::string content = editor.GetText();
			SaveFileContent(filePath, content);

			if (filePath == context->programs.at(programNum).vsPath) {
				context->programs.at(programNum) = Program(filePath.c_str(), context->programs.at(programNum).fsPath.c_str());
			}
			else {
				context->programs.at(programNum) = Program(context->programs.at(programNum).vsPath.c_str(), filePath.c_str());
			}
		}
	}
}

void ShaderEditorPanel::DrawFileName(std::string name, ImVec4 color) {

	ImGui::SameLine();
	//ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_Text, color);
	TextCentered(name.c_str());
	ImGui::PopStyleColor();
}

void ShaderEditorPanel::FileBtns() {

	if (ImGui::Button("Open", ImVec2(50, 20))) {

		filePath = OpenFile("Shaders (*.glsl)\0*.glsl\0");
		fileName = GetFileName(filePath);

		std::string content = GetFileContent(filePath);
		editor.SetText(content);
	}

	ImGui::SameLine();

	if (ImGui::Button("Save As", ImVec2(60, 20))) {

		filePath = SaveFile("Shaders (*.glsl)\0*.glsl\0");
		//fileName = GetFileName(filePath);

		std::string content = editor.GetText();
		SaveFileContent(filePath, content);

		//context->InitShaders("lightingPass_vs.glsl", filePath.c_str());            //programs[0]
		//context->pipeline->lStep->SetProgram(context->programs.at(context->programs.size() - 1));
	}
}

void ShaderEditorPanel::TextCentered(std::string text) {

	float win_width = ImGui::GetWindowSize().x + 50; //+ 50 from the buttons
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

std::string ShaderEditorPanel::SaveFile(const char* filter) {

	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&ofn) == TRUE) {
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

void ShaderEditorPanel::SaveFileContent(std::string path, std::string content) {

	std::ofstream t(path);
	t << content;
}