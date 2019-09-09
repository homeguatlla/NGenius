#include "stdafx.h"
#include "IndicesRenderer.h"
#include "../models/Model.h"
#include <GL/glew.h>


IndicesRenderer::IndicesRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
	SetCullingEnabled(true);
	SetBlendingEnabled(false);
	SetDepthBufferEnabled(true);
	SetDepthBufferValues(true, GL_LESS);
}

IndicesRenderer::~IndicesRenderer()
{
}
/*
const std::string IndicesRenderer::GetName() const
{
	return std::string("IndicesRenderer") + std::to_string(mModel->GetID());
}*/

bool IndicesRenderer::IsInstancingAllowed() const
{
	return true;
}