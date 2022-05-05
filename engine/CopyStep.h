#pragma once

#ifndef COPYSTEP_H
#define COPYSTEP_H

#include "Step.h"

class CopyStep : public Step
{
public:

	CopyStep(GLbitfield mask, unsigned int* drawBuffer, unsigned int width, unsigned int height);

	void RenderStep() override;
private:

	GLbitfield mask;

	unsigned int* drawBuffer;

	const unsigned int width;
	const unsigned int height;
};

#endif