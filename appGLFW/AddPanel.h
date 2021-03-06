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

	char diffusePath[256];
	char specPath[256];
	char emissivePath[256];
	void AddCube();

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	void AddLight();

	char modelPath[256];
	void AddModel();

	float speed = 1.0f;
	float fov = 45.0f;
	float yaw = -90.0;
	float pitch = 0.0f;
	void AddCamera();
};

#endif