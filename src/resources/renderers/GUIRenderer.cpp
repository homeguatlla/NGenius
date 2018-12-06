#include "stdafx.h"
#include "GUIRenderer.h"
#include <GL/glew.h>

GUIRenderer::GUIRenderer(Model* model, IMaterial* material) :
	TextRenderer(model, material),
	mSourceFactor(GL_SRC_ALPHA),
	mDestinationFactor(GL_ONE_MINUS_SRC_ALPHA),
	mIsBlendingEnabled(true),
	mIsDepthTestEnabled(true)
{
	SetLayer(IRenderer::LAYER_GUI);
}


GUIRenderer::~GUIRenderer()
{
}

void GUIRenderer::SetBlendFactors(int source, int destination)
{
	mSourceFactor = source;
	mDestinationFactor = destination;
}

bool GUIRenderer::IsBlendingEnabled() const
{
	return mIsBlendingEnabled;
}

void GUIRenderer::SetBlendEnabled(bool enabled)
{
	mIsBlendingEnabled = enabled;
}

void GUIRenderer::SetDepthTestEnabled(bool enabled)
{
	mIsDepthTestEnabled = enabled;
}
bool GUIRenderer::IsDepthTestEnabled() const
{
	return mIsDepthTestEnabled;
}

void GUIRenderer::Draw()
{
	glDisable(GL_CULL_FACE);
	if (!IsDepthTestEnabled())
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(false);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(true);
	}

	if (IsBlendingEnabled())
	{
		glEnable(GL_BLEND);
		glBlendFunc(mSourceFactor, mDestinationFactor);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

	IRenderer::Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}
