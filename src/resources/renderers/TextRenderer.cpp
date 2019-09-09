#include "stdafx.h"
#include "TextRenderer.h"

#include "../renderers/IRenderer.h"
#include "../font/FontType.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"
#include "../models/Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

TextRenderer::TextRenderer(Model* model, IMaterial* material) : 
IRenderer(model, material)
{
	SetLayer(IRenderer::LAYER_PARTICLES);
	SetTransparency(true);
	SetCullingEnabled(false);
	SetBlendingEnabled(true);
	SetBlendingFactors(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

TextRenderer::~TextRenderer()
{
}

bool TextRenderer::IsInstancingAllowed() const
{
	return false;
}

void TextRenderer::Draw()
{
	glDepthMask(false);
	
	IRenderer::Draw();

	glDepthMask(true);
}