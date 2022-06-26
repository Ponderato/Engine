#pragma once

#ifndef PIPELINE_H
#define PIPELINE_H

#include <vector>
#include "Step.h"
#include "GeometryStep.h"
#include "LightingStep.h"
#include "CopyStep.h"
#include "ForwardStep.h"
#include "FrustumStep.h"

class Pipeline 
{
public:

	GeometryStep* gStep;
	LightingStep* lStep;
	ForwardStep* fStep;
	CopyStep* cStep;
	FrustumStep* frusStep;

	void SetStep(Step* step);
	void Render();
};

#endif