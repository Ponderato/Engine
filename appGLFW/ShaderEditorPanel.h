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

	void OnImGuiRender() override;

private:

	TextEditor editor;

};

#endif