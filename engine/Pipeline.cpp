#include "Pipeline.h"

Pipeline::Pipeline(std::string Name) : Name(Name){}

//Makes the given step the active one
void Pipeline::SetStep(Step* step) {

	if (GeometryStep* stepAux = dynamic_cast<GeometryStep*>(step)) {
		this->gStep = stepAux;
	}
	else if (LightingStep* stepAux = dynamic_cast<LightingStep*>(step)) {
		this->lStep = stepAux;
	}
	else if (ForwardStep* stepAux = dynamic_cast<ForwardStep*>(step)) {
		this->fStep = stepAux;
	}
	else if (CopyStep* stepAux = dynamic_cast<CopyStep*>(step)) {
		this->cStep = stepAux;
	}
	else if (FrustumStep* stepAux = dynamic_cast<FrustumStep*>(step)) {
		this->frusStep = stepAux;
	}
}

void Pipeline::Render() {

	if(gStep != nullptr)
		gStep->RenderStep();

	if (lStep != nullptr)
		lStep->RenderStep();

	if (cStep != nullptr)
		cStep->RenderStep();

	if (fStep != nullptr)
		fStep->RenderStep();

	if (frusStep != nullptr)
		frusStep->RenderStep();

}

