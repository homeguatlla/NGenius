#include "stdafx.h"
#include "IndexesRenderer.h"
#include "../models/Model.h"


IndexesRenderer::IndexesRenderer(Model* model, IMaterial* material) :
IRenderer_(model, material)
{
}

IndexesRenderer::~IndexesRenderer()
{
}
/*
const std::string IndexesRenderer::GetName() const
{
	return std::string("IndexesRenderer") + std::to_string(mModel->GetID());
}*/

bool IndexesRenderer::IsInstancingAllowed() const
{
	return true;
}