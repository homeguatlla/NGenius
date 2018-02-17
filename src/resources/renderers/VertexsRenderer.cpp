#include "stdafx.h"
#include "VertexsRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../camera/ICamera.h"
#include <GL/glew.h>


VertexsRenderer::VertexsRenderer(Model* model, IMaterial* material) :
IRenderer_(model, material)
{
}

VertexsRenderer::~VertexsRenderer()
{
}

const std::string VertexsRenderer::GetName() const
{
	return std::string("VertexsRenderer") + std::to_string(mModel->GetID());
}

void VertexsRenderer::Draw()
{
	if (mIsInstancingEnabled)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, mModel->GetNumberOfVertexs(), mInstances.size());
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, mModel->GetNumberOfVertexs());
	}
}

bool VertexsRenderer::IsInstancingAllowed() const
{
	return true;
}