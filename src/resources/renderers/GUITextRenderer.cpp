#include "stdafx.h"
#include "GUITextRenderer.h"
#include <GL/glew.h>

GUITextRenderer::GUITextRenderer(Model* model, IMaterial* material) :
	TextRenderer(model, material),
	mSourceFactor(GL_SRC_ALPHA),
	mDestinationFactor(GL_ONE_MINUS_SRC_ALPHA)
{
	SetLayer(IRenderer::LAYER_GUI);
}


GUITextRenderer::~GUITextRenderer()
{
}

void GUITextRenderer::SetBlendFactors(int source, int destination)
{
	mSourceFactor = source;
	mDestinationFactor = destination;
}

void GUITextRenderer::Draw()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(true);
	glEnable(GL_BLEND);
	glBlendFunc(mSourceFactor, mDestinationFactor);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

	IRenderer::Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}
