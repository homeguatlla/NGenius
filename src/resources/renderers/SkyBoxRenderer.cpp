#include "stdafx.h"
#include "SkyBoxRenderer.h"
#include "../models/Model.h"
#include "../models/Mesh.h"
#include <GL/glew.h>

SkyBoxRenderer::SkyBoxRenderer(Model* model, IMaterial* material) :
	IRenderer(model, material)
{
	SetCullingEnabled(true);
	SetBlendingEnabled(false);
	SetDepthBufferEnabled(true);
	SetDepthBufferValues(true, GL_LESS);
}


SkyBoxRenderer::~SkyBoxRenderer()
{
}

bool SkyBoxRenderer::IsInstancingAllowed() const
{
	return false;
}