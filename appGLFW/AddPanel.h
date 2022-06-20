#pragma once

#ifndef ADDPANEL_H
#define ADDPANEL_H

#include "Panel.h"

class AddPanel : public Panel
{
public:

	bool close = false;

	AddPanel() = default;
	AddPanel(Context* context);

	void OnImGuiRender() override;

private:

	void AddCube();

	char diffusePath[256];
	char specPath[256];
	char emissivePath[256];


};

#endif