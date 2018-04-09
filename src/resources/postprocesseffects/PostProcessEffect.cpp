#include "stdafx.h"
#include "PostProcessEffect.h"
#include "../renderers/ImageRenderer.h"
#include "../systems/renderSystem/IFrameBuffer.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../textures/Texture.h"

PostProcessEffect::PostProcessEffect(PostProcessEffectType type, Texture* texture, ImageRenderer* imageRenderer) :
	mType(type),
	mWidth(0),
	mHeight(0),
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
	if (mType == PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER)
	{
		/*mFrameBuffer = new IFrameBuffer(mWidth, mHeight);
		mFrameBuffer->SetColorTextureAttachment(0, mTexture);
		mFrameBuffer->Init();*/

		IMaterial* material = mRenderer->GetMaterial();
		mMaterialEffectDiffuseTexture = new MaterialEffectDiffuseTexture(mTexture, glm::vec3(0.0f), 0);
		material->AddEffect(mMaterialEffectDiffuseTexture);
	}
}

void PostProcessEffect::SetBufferSize(unsigned int width, unsigned int height)
{
	mWidth = width;
	mHeight = height;
}

bool PostProcessEffect::HasFrameBuffer()
{
	return mType == PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER;
}

ITexture* PostProcessEffect::Render(ITexture* texture)
{
	if (HasFrameBuffer())
	{
		//mFrameBuffer->BindBuffer();
		//mMaterialEffectDiffuseTexture->SetDiffuseTexture(texture);
		mRenderer->Render();
		//mFrameBuffer->UnbindBuffer();
	}
	else
	{
		mRenderer->Render();
	}
	
	return texture;
}