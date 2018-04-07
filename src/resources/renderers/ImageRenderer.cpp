#include "stdafx.h"
#include "ImageRenderer.h"
#include "../materials/IMaterial.h"
#include "../models/Model.h"
#include <GL/glew.h>
#include <vector>

ImageRenderer::ImageRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
}

ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::Render()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(mModel->GetVAOID());

	mMaterial->Use();

	mMaterial->Apply(nullptr, nullptr);
	
	Draw();

	mMaterial->UnUse();

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

bool ImageRenderer::IsInstancingAllowed() const
{
	return false;
}