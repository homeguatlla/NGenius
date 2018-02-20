#include "stdafx.h"
#include "TextRenderer.h"

#include "../renderers/IRenderer_.h"
#include "../font/FontType.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"
#include "../models/Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

TextRenderer::TextRenderer(Model* model, IMaterial* material) : 
IRenderer_(model, material)
{
	SetLayer(IRenderer_::LAYER_PARTICLES);
	SetTransparency(true);
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
	glDisable(GL_CULL_FACE);
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	IRenderer_::Draw();

	glDisable(GL_BLEND);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
}