#pragma once

#ifndef COPYSTEP_H
#define COPYSTEP_H

#include "Step.h"

class CopyStep : public Step
{
public:

	CopyStep() {};

	void RenderStep(unsigned int* readBuffer, unsigned int* drawBuffer, GLbitfield mask, unsigned int width, unsigned int height) override;
};

#endif