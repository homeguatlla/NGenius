#include "stdafx.h"
#include "PointsRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../camera/ICamera.h"
#include <GL/glew.h>


PointsRenderer::PointsRenderer(Model* model, IMaterial* material) : IRenderer(model, material)
{
	SetLayer(IRenderer::LAYER_TRANSPARENT);
	SetTransparency(false);
}

PointsRenderer::~PointsRenderer()
{
}

void PointsRenderer::Draw()
{
	//glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (mIsInstancingEnabled)
	{
		glDrawArraysInstanced(GL_POINTS, 0, mModel->GetNumberOfVertexs(), mInstances.size());
	}
	else
	{
		glDrawArrays(GL_POINTS, 0, mModel->GetNumberOfVertexs());
	}

	glDisable(GL_BLEND);
	//glDepthMask(true);
}

bool PointsRenderer::IsInstancingAllowed() const
{
	return true;
}

unsigned int PointsRenderer::GetNumberTrianglesRendered() const
{
	return mInstances.size() * mModel->GetNumberOfVertexs() / 3;
}