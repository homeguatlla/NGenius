#include "stdafx.h"
#include "PostProcessEffect.h"
#include "../renderers/ImageRenderer.h"
#include "../systems/renderSystem/IFrameBuffer.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../textures/Texture.h"

#include <GL\glew.h>

PostProcessEffect::PostProcessEffect(PostProcessEffectType type, Texture* outTexture, ImageRenderer* imageRenderer) :
	mType(type),
	mWidth(0),
	mHeight(0),
	mFrameBuffer(nullptr),
	mOutTexture(outTexture),
	mRenderer(imageRenderer)
{
	assert(outTexture != nullptr);
	assert(imageRenderer != nullptr);
}

PostProcessEffect::~PostProcessEffect()
{
	delete mRenderer;
}

void PostProcessEffect::Init()
{
	switch (mType)
	{
		case PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER:
			mFrameBuffer = new IFrameBuffer(mWidth, mHeight);
			mFrameBuffer->SetColorTextureAttachment(0, mOutTexture);
			mFrameBuffer->Init();
			break;
		case PostProcessEffectType::POSTPROCESS_TO_SCREEN:
			break;
		default:
			break;
	}

	//Creating the materialEffectDiffuse with the Input Texture equal to the Output Texture only because of creation
	IMaterial* material = mRenderer->GetMaterial();
	assert(material != nullptr);

	mMaterialEffectDiffuseTexture = new MaterialEffectDiffuseTexture(mOutTexture, glm::vec3(0.0f), 0);
	material->AddOrReplaceEffect(mMaterialEffectDiffuseTexture);
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

ITexture* PostProcessEffect::Render(ITexture* inTexture)
{
	mMaterialEffectDiffuseTexture->SetDiffuseTexture(inTexture);

	if (HasFrameBuffer())
	{
		mFrameBuffer->BindBuffer();
		mRenderer->Render();
		mFrameBuffer->UnbindBuffer();
	}
	else
	{
		glDrawBuffer(GL_BACK);
		glViewport(0, 0, mWidth, mHeight);
		mRenderer->Render();
	}
	
	return mOutTexture;
}