#include "stdafx.h"
#include "PostProcessSubSystem.h"
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

void PostProcessSubSystem::LoadContrastPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("contrast", mRenderSystem->GetShader("contrast"));
	material->AddEffect(new MaterialEffectFloat(0.3f));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(type, texture,	renderer);

	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadHorizontalBlurPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, float blurGranularity, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("horizontal_blur", mRenderSystem->GetShader("horizontal_blur"));
	material->AddEffect(new MaterialEffectFloat(blurGranularity));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(type, texture, renderer);
	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadVerticalBlurPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, float blurGranularity, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("vertical_blur", mRenderSystem->GetShader("vertical_blur"));
	material->AddEffect(new MaterialEffectFloat(blurGranularity));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(type, texture,	renderer);
	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::Load()
{
	unsigned int screenWidth = static_cast<unsigned int>(mRenderSystem->GetScreenWidth());
	unsigned int screenHeight = static_cast<unsigned int>(mRenderSystem->GetScreenHeight());

	Texture* texture = static_cast<Texture*>(mRenderSystem->CreateColorTexture("postprocess", glm::vec2(screenWidth, screenHeight)));
	Texture* texture2 = static_cast<Texture*>(mRenderSystem->CreateColorTexture("postprocess2", glm::vec2(screenWidth, screenHeight)));
	
	Model* model = mRenderSystem->GetModel("quad");
	
	LoadContrastPostProcess(model, texture2, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER, screenWidth, screenHeight);
	LoadHorizontalBlurPostProcess(model, texture, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER, screenWidth / 2.0f, screenWidth, screenHeight);
	LoadVerticalBlurPostProcess(model, texture2, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER, screenHeight/ 2.0f, screenWidth, screenHeight);
	LoadHorizontalBlurPostProcess(model, texture, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER, screenWidth / 4.0f, screenWidth, screenHeight);
	LoadVerticalBlurPostProcess(model, texture2, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_SCREEN, screenHeight / 4.0f, screenWidth, screenHeight);

	if (!model->IsBuilt())
	{
		//TODO esto se puede hacer con un material por defecto en lugar de este
		model->Build(mRenderSystem->GetVertexBufferManager(), mRenderSystem->GetMaterial("contrast"));
	}

	//creating frame buffer
	mFrameBuffer = new IFrameBuffer(screenWidth, screenHeight);
	mFrameBuffer->SetCopyBufferToTexture(texture, 0, 0, texture->GetWidth(), texture->GetHeight());
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
