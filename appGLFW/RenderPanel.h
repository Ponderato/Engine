#pragma once

#ifndef RENDERPANEL_H
#define RENDERPANEL_H

#include "Panel.h"

class RenderPanel : public Panel
{
public:

	RenderPanel() = default;
	RenderPanel(Context* context);

	void OnImGuiRender() override;

	inline void SetRenderImage(const unsigned int renderImg) { this->renderImg = renderImg; };
	inline void SetFPS(int fps) { this->fps = fps; };
	inline void SetMS(double ms) { this->ms = ms; };

private:

	int fps = 0;
	double ms = 0.0;
	unsigned int renderImg;

	void DrawData();
};

#endif