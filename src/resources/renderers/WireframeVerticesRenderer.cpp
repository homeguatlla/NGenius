#include "stdafx.h"
#include "WireframeVerticesRenderer.h"
#include "src/resources/models/Model.h"
#include <GL/glew.h>

WireframeVerticesRenderer::WireframeVerticesRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
	SetDepthBufferEnabled(true);
	SetDepthBufferValues(true, GL_LESS);
}

WireframeVerticesRenderer::~WireframeVerticesRenderer()
{
}

void WireframeVerticesRenderer::Draw()
{
	if (mIsInstancingEnabled)
	{
		glDrawArraysInstanced(GL_LINES, 0, mModel->GetNumberOfVertexs(), mInstances.size());
	}
	else
	{
		glDrawArrays(GL_LINES, 0, mModel->GetNumberOfVertexs());
	}
}

bool WireframeVerticesRenderer::IsInstancingAllowed() const
{
	return true;
}

unsigned int WireframeVerticesRenderer::GetNumberTrianglesRendered() const
{
	return mInstances.size() * mModel->GetNumberOfIndexes() / 3;
}