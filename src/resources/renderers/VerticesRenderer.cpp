#include "stdafx.h"
#include "VerticesRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../camera/ICamera.h"
#include <GL/glew.h>


VerticesRenderer::VerticesRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
}

VerticesRenderer::~VerticesRenderer()
{
}
/*
const std::string VerticesRenderer::GetName() const
{
	return std::string("VerticesRenderer") + std::to_string(mModel->GetID());
}*/

void VerticesRenderer::Draw()
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

bool VerticesRenderer::IsInstancingAllowed() const
{
	return true;
}

unsigned int VerticesRenderer::GetNumberTrianglesRendered() const
{
	return mInstances.size() * mModel->GetNumberOfVertexs() / 3;
}