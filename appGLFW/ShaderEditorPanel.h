#pragma once

#ifndef SHADEREDITORPANEL_H
#define SHADEREDITORPANEL_H

#include "Panel.h"
#include "TextEditor.h"

class ShaderEditorPanel : public Panel
{
public:

	ShaderEditorPanel() = default;
	ShaderEditorPanel(Context* context);

	void ConfigureEditor();

	void OnImGuiRender() override;

private:

	std::string filePath;
	std::string fileName = "No File Opened";
	TextEditor editor;

	void SelectFile();
	void SaveFileBtn();
	void DrawFileName(std::string name, ImVec4 color);
	void TextCentered(std::string text);

	std::string OpenFile(const char* filter);
	std::string SaveFile(const char* filter);

	std::string GetFileName(std::string path);
	std::string GetFileContent(std::string path);
	void SaveFileContent(std::string path, std::string content);
	
};

#endif