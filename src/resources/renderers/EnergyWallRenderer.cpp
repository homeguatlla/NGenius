#include "stdafx.h"
#include "EnergyWallRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../camera/ICamera.h"
#include <GL/glew.h>


EnergyWallRenderer::EnergyWallRenderer(Model* model, IMaterial* material) :
VertexsRenderer(model, material)
{
}

EnergyWallRenderer::~EnergyWallRenderer()
{
}

void EnergyWallRenderer::Draw()
{
	glDisable(GL_CULL_FACE);
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);// GL_ONE_MINUS_SRC_ALPHA);
	
	VertexsRenderer::Draw();

	glDisable(GL_BLEND);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
}

bool EnergyWallRenderer::IsInstancingAllowed() const
{
	return true;
}