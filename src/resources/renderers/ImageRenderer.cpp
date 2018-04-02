#include "stdafx.h"
#include "ImageRenderer.h"
#include "../materials/IMaterial.h"
#include "../models/Model.h"
#include <GL/glew.h>

ImageRenderer::ImageRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
}

ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::Render()
{
	glBindVertexArray(mModel->GetVAOID());

	mMaterial->Apply(nullptr, nullptr);

	Draw();

	glBindVertexArray(0);
}

bool ImageRenderer::IsInstancingAllowed() const
{
	return false;
}