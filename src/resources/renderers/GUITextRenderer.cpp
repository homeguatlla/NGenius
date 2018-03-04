#include "stdafx.h"
#include "GUITextRenderer.h"
#include <GL/glew.h>

GUITextRenderer::GUITextRenderer(Model* model, IMaterial* material) :
	TextRenderer(model, material)
{
	SetLayer(IRenderer::LAYER_GUI);
}


GUITextRenderer::~GUITextRenderer()
{
}

void GUITextRenderer::Draw()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IRenderer::Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
}
