#include "stdafx.h"
#include "IndicesRenderer.h"
#include "../models/Model.h"


IndicesRenderer::IndicesRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
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