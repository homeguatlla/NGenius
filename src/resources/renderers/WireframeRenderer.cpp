#include "stdafx.h"
#include "WireframeRenderer.h"
#include "../models/Model.h"
#include <GL/glew.h>

WireframeRenderer::WireframeRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
	SetDepthBufferEnabled(true);
	SetDepthBufferValues(true, GL_LESS);
}

WireframeRenderer::~WireframeRenderer()
{
}

void WireframeRenderer::Draw()
{
	if (mIsInstancingEnabled)
	{
		glDrawElementsInstancedARB(GL_LINES, mModel->GetNumberOfIndexes(), GL_UNSIGNED_INT, 0, mInstances.size());
	}
	else
	{
		glDrawElements(GL_LINES, mModel->GetNumberOfIndexes(), GL_UNSIGNED_INT, 0);
	}
}

bool WireframeRenderer::IsInstancingAllowed() const
{
	return true;
}

unsigned int WireframeRenderer::GetNumberTrianglesRendered() const
{
	return 0;
}