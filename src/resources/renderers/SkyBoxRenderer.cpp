#include "stdafx.h"
#include "SkyBoxRenderer.h"
#include "../models/Model.h"
#include "../models/Mesh.h"
#include <GL/glew.h>

SkyBoxRenderer::SkyBoxRenderer(Model* model, IMaterial* material) :
	IRenderer_(model, material)
{
}


SkyBoxRenderer::~SkyBoxRenderer()
{
}

bool SkyBoxRenderer::IsInstancingAllowed() const
{
	return false;
}

void SkyBoxRenderer::Draw()
{
	//draw the skybox last
	//glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	IRenderer_::Draw();

	glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
}