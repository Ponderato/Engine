#pragma once

#ifndef PANEL_H
#define PANEL_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Context.h"

class Panel
{
public:
	
	Panel() = default;
	Panel(const Context& context);

	inline void SetContext(const Context& context) { this->context = context; };

	virtual void OnImGuiRender() = 0;
protected:

	Context context;
};

#endif