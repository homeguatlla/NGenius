#include "stdafx.h"
#include "EnergyWallRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../camera/ICamera.h"
#include <GL/glew.h>


EnergyWallRenderer::EnergyWallRenderer(Model* model, IMaterial* material) :
VerticesRenderer(model, material)
{
	SetBlendingEnabled(true);
	SetBlendingFactors(GL_SRC_ALPHA, GL_ONE);
	SetCullingEnabled(false);
}

EnergyWallRenderer::~EnergyWallRenderer()
{
}

void EnergyWallRenderer::Draw()
{
	glDepthMask(false);
	
	VerticesRenderer::Draw();

	glDepthMask(true);
}

bool EnergyWallRenderer::IsInstancingAllowed() const
{
	return true;
}