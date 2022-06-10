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
private:

	unsigned int renderImg;
};

#endif