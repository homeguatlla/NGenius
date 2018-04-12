#include "stdafx.h"
#include "PostProcessSubSystem.h"
#include "../../postprocesseffects/PostProcessEffect.h"
#include "../renderSystem/RenderSystem.h"
#include "../../renderers/ImageRenderer.h"
#include "../../models/Model.h"
#include "../../textures/Texture.h"
#include "../../materials/IMaterial.h"
#include "../../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../materials/effects/MaterialEffectFloat.h"
#include "../../systems/renderSystem/IFrameBuffer.h"

#include <GL/glew.h>

PostProcessSubSystem::PostProcessSubSystem(RenderSystem* renderSystem) :
	mRenderSystem(renderSystem),
	mFrameBuffer(nullptr)
{
}

PostProcessSubSystem::~PostProcessSubSystem()
{
	if (mFrameBuffer != nullptr)
	{
		delete mFrameBuffer;
	}
	for (PostProcessEffect* effect : mEffects)
	{
		delete effect;
	}
	mEffects.clear();
}

void PostProcessSubSystem::Init()
{
	Load();

	for (PostProcessEffect* postProcessEffect : mEffects)
	{
		postProcessEffect->Init();
	}
}

void PostProcessSubSystem::LoadContrastPostProcess(Model* model, Texture* texture)
{
	IMaterial* material = mRenderSystem->CreateMaterial("contrast", mRenderSystem->GetShader("contrast"));
	material->AddEffect(new MaterialEffectFloat(0.3f));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(	PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER,
																	texture,
																	renderer);
	/*
	postProcessEffect->SetBufferSize(	static_cast<unsigned int>(mRenderSystem->GetScreenWidth()),
	static_cast<unsigned int>(mRenderSystem->GetScreenHeight()));
	*/
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadHorizontalBlurPostProcess(Model* model, Texture* texture, float width)
{
	IMaterial* material = mRenderSystem->CreateMaterial("horizontal_blur", mRenderSystem->GetShader("horizontal_blur"));
	material->AddEffect(new MaterialEffectFloat(width));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER,
		texture,
		renderer);
	/*
	postProcessEffect->SetBufferSize(	static_cast<unsigned int>(mRenderSystem->GetScreenWidth()),
	static_cast<unsigned int>(mRenderSystem->GetScreenHeight()));
	*/
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadVerticalBlurPostProcess(Model* model, Texture* texture, float height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("vertical_blur", mRenderSystem->GetShader("vertical_blur"));
	material->AddEffect(new MaterialEffectFloat(height));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER,
		texture,
		renderer);
	/*
	postProcessEffect->SetBufferSize(	static_cast<unsigned int>(mRenderSystem->GetScreenWidth()),
	static_cast<unsigned int>(mRenderSystem->GetScreenHeight()));
	*/
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::Load()
{
	Texture* texture = static_cast<Texture*>(mRenderSystem->GetTexture("postprocess"));
	Model* model = mRenderSystem->GetModel("quad");
	
	//LoadContrastPostProcess(model, texture);
	//LoadHorizontalBlurPostProcess(model, texture, 1024.0f);
	LoadVerticalBlurPostProcess(model, texture, 768.0f);

	if (!model->IsBuilt())
	{
		//TODO esto se puede hacer con un material por defecto en lugar de este
		model->Build(mRenderSystem->GetVertexBufferManager(), mRenderSystem->GetMaterial("vertical_blur"));
	}

	//creating frame buffer
	unsigned int screenWidth = static_cast<unsigned int>(mRenderSystem->GetScreenWidth());
	unsigned int screenHeight = static_cast<unsigned int>(mRenderSystem->GetScreenHeight());
	mFrameBuffer = new IFrameBuffer(screenWidth, screenHeight);
	mFrameBuffer->SetCopyBufferToTexture(texture, 0, 0, screenWidth, screenHeight);
	//mFrameBuffer->Init();
}

void PostProcessSubSystem::Render(ITexture* texture)
{
	texture = mFrameBuffer->CopyColorBuffer();
	glDisable(GL_DEPTH_TEST);
	for (PostProcessEffect* postProcessEffect : mEffects)
	{
		texture = postProcessEffect->Render(texture);
	}
	glEnable(GL_DEPTH_TEST);
}

void PostProcessSubSystem::AddPostProcessEffect(PostProcessEffect* postProcessEffect)
{
	bool found = std::find(mEffects.begin(), mEffects.end(), postProcessEffect) != mEffects.end();
	if (!found)
	{
		mEffects.push_back(postProcessEffect);
	}
}

void PostProcessSubSystem::RemovePostProcessEffect(PostProcessEffect* postProcessEffect)
{
	PostProcessEffectIterator it = std::find(mEffects.begin(), mEffects.end(), postProcessEffect);
	bool found = it != mEffects.end();
	if (found)
	{
		mEffects.erase(it);
	}
}
