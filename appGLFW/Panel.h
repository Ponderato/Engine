#pragma once

#ifndef PANEL_H
#define PANEL_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <imgui_internal.h>
#include <gtc/type_ptr.hpp>

#include "Context.h"

class Panel
{
public:
	
	Panel() = default;
	Panel(Context* context);

	inline void SetContext(Context* context) { this->context = context; };

	void DrawVec3(const std::string& label, glm::vec3* values, float reset = 0.0f, float columnWidth = 70.0f);
	void DrawVec3RGB(const std::string& label, glm::vec3* values, float reset = 0.0f, float columnWidth = 70.0f);
	void DrawFloat(const std::string& label, float* values, ImVec4 color, float reset = 0.0f, float columnWidth = 70.0f);
	void DrawTag(char buffer[]);

	virtual void OnImGuiRender() = 0;
protected:

	Context* context;
};

#endif