#include "stdafx.h"
#include "ParticleRenderer.h"

#include "../renderers/IRenderer.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"
#include "../models/Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ParticleRenderer::ParticleRenderer(Model* model, IMaterial* material) :
IRenderer(model, material)
{
	SetLayer(IRenderer::LAYER_PARTICLES);
	SetTransparency(true);
	SetBillboard(true);
	SetCullingEnabled(false);
	SetBlendingEnabled(true);
	SetBlendingFactors(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

ParticleRenderer::~ParticleRenderer()
{
}

bool ParticleRenderer::IsInstancingAllowed() const
{
	return true;
}

void ParticleRenderer::Draw()
{
	glDepthMask(false);
		
	IRenderer::Draw();

	glDepthMask(true);
}