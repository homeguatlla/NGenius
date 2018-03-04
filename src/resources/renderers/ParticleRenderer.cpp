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
	glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	IRenderer::Draw();

	glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
}