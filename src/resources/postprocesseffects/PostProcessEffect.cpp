#include "stdafx.h"
#include "PostProcessEffect.h"
#include "../renderers/ImageRenderer.h"
#include "../systems/renderSystem/IFrameBuffer.h"

PostProcessEffect::PostProcessEffect(Texture* texture, ImageRenderer* imageRenderer, unsigned int width, unsigned int height) :
	mWidth(width),
	mHeight(height),
	mTransformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
	mFrameBuffer(nullptr),
	mTexture(texture),
	mRenderer(imageRenderer)
{
	
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

	mTransformation.SetScale(glm::vec3(mWidth, mHeight, 1.0f));
	mRenderer->SetTransformation(mTransformation);
}

unsigned int PostProcessEffect::Render(unsigned int texture)
{
	mFrameBuffer->BindBuffer();
	mRenderer->Render();
	mFrameBuffer->UnbindBuffer();

	return texture;
}