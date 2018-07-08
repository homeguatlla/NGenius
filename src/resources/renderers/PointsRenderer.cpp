#include "stdafx.h"
#include "PointsRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../camera/ICamera.h"
#include <GL/glew.h>


PointsRenderer::PointsRenderer(Model* model, IMaterial* material) : IRenderer(model, material)
{
}

PointsRenderer::~PointsRenderer()
{
}

void PointsRenderer::Draw()
{
	if (mIsInstancingEnabled)
	{
		glDrawArraysInstanced(GL_POINTS, 0, mModel->GetNumberOfVertexs(), mInstances.size());
	}
	else
	{
		glDrawArrays(GL_POINTS, 0, mModel->GetNumberOfVertexs());
	}
}

bool PointsRenderer::IsInstancingAllowed() const
{
	return true;
}

unsigned int PointsRenderer::GetNumberTrianglesRendered() const
{
	return mInstances.size() * mModel->GetNumberOfVertexs() / 3;
}