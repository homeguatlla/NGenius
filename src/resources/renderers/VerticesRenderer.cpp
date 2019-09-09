#include "stdafx.h"
#include "VerticesRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../camera/ICamera.h"
#include "../Memory.h"
#include <GL/glew.h>


VerticesRenderer::VerticesRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
	SetDepthBufferEnabled(true);
	SetDepthBufferValues(true, GL_LESS);
}

VerticesRenderer::~VerticesRenderer()
{
}

VerticesRenderer* VerticesRenderer::DoClone() const
{
	VerticesRenderer* renderer = DBG_NEW VerticesRenderer(mModel, mMaterial);
	renderer->SetBillboard(mIsBillboard);
	renderer->SetCullingEnabled(mIsCullingEnabled);
	renderer->SetLayer(mLayer);
	renderer->SetParent(mParent);

	return renderer;
}

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