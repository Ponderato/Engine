#pragma once

#ifndef COPYSTEP_H
#define COPYSTEP_H

#include "Step.h"

class CopyStep : public Step
{
public:

	CopyStep(GLbitfield mask, unsigned int width, unsigned int height);

	void RenderStep(unsigned int& readBuffer, unsigned int& drawBuffer) override;
private:

	GLbitfield mask;

	const unsigned int width;
	const unsigned int height;
};

#endif