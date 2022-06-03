#pragma once

#ifndef HIERARCHYPANEL_H
#define HIERARCHYPANEL_H

#include "Panel.h"

class HierarchyPanel : public Panel
{
public:

	HierarchyPanel() = default;
	HierarchyPanel(const Context& context);

	void OnImGuiRender() override;
};

#endif