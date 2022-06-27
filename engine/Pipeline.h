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

	std::string Name;

	GeometryStep* gStep = nullptr;
	LightingStep* lStep = nullptr;
	ForwardStep* fStep = nullptr;
	CopyStep* cStep = nullptr;
	FrustumStep* frusStep = nullptr;

	Pipeline(std::string Name);

	void SetStep(Step* step);
	void Render();


};

#endif