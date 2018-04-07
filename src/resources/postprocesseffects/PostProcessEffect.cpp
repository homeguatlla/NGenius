#include "stdafx.h"
#include "PostProcessEffect.h"
#include "../renderers/ImageRenderer.h"
#include "../systems/renderSystem/IFrameBuffer.h"

PostProcessEffect::PostProcessEffect(Texture* texture, ImageRenderer* imageRenderer, unsigned int width, unsigned int height) :
	mWidth(width),
	mHeight(height),
	mFrameBuffer(nullptr),
	mTexture(texture),
	mRenderer(imageRenderer)
{
	assert(texture != nullptr);
	assert(imageRenderer != nullptr);
}

PostProcessEffect::~PostProcessEffect()
{
	delete mRenderer;
}

void PostProcessEffect::Init()
{
	mFrameBuffer = new IFrameBuffer(mWidth, mHeight);
	mFrameBuffer->SetColorTextureAttachment(0, mTexture);
	mFrameBuffer->Init();
}

unsigned int PostProcessEffect::Render(unsigned int texture)
{
	mFrameBuffer->BindBuffer();
	mRenderer->Render();
	mFrameBuffer->UnbindBuffer();

	return texture;
}