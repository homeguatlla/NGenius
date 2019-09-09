#include "stdafx.h"
#include "GUITextRenderer.h"
#include <GL/glew.h>

GUITextRenderer::GUITextRenderer(Model* model, IMaterial* material) :
	TextRenderer(model, material)
{
	SetLayer(IRenderer::LAYER_GUI);
	SetCullingEnabled(false);
	SetBlendingEnabled(true);
	SetBlendingFactors(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


GUITextRenderer::~GUITextRenderer()
{
}

void GUITextRenderer::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

	IRenderer::Draw();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
}
