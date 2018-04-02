#include "stdafx.h"
#include "PostProcessEffect.h"
#include "../renderers/ImageRenderer.h"

PostProcessEffect::PostProcessEffect(ImageRenderer* imageRenderer, unsigned int width, unsigned int height) :
	mRenderer(imageRenderer),
	mWidth(width),
	mHeight(height)
{
}

PostProcessEffect::~PostProcessEffect()
{
	delete mRenderer;
}

void PostProcessEffect::Init()
{
}

unsigned int PostProcessEffect::Render(unsigned int texture)
{
	mRenderer->Render();

	return texture;
}