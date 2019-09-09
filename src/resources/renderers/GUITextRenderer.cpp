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
	SetDepthBufferEnabled(false);
}


GUITextRenderer::~GUITextRenderer()
{
}
