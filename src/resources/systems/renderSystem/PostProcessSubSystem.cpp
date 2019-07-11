#include "stdafx.h"
#include "PostProcessSubSystem.h"
#include "../renderSystem/RenderSystem.h"
#include "../../renderers/ImageRenderer.h"
#include "../../models/Model.h"
#include "../../textures/Texture.h"
#include "../../materials/IMaterial.h"
#include "../../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../materials/effects/MaterialEffectDepthTexture.h"
#include "../../materials/effects/MaterialEffectFloat.h"
#include "../../materials/effects/MaterialEffectFloat3.h"
#include "../../systems/renderSystem/IFrameBuffer.h"

#include <GL/glew.h>

//const static glm::vec3 BRIGHT_VECTOR(0.2126, 0.7152, 0.0722);
const static glm::vec3 BRIGHT_VECTOR(0.033, 0.033, 0.033);

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
	ReleaseEffects();
}

void PostProcessSubSystem::ReleaseEffects()
{
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
	IMaterial* material = mRenderSystem->CreateMaterial("contrast", mRenderSystem->GetShader("ContrastShader"));
	material->AddOrReplaceEffect(new MaterialEffectFloat(0.3f));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(type, texture,	renderer);

	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadBrightPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, const glm::vec3& bright, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("bright", mRenderSystem->GetShader("BrightShader"));
	material->AddOrReplaceEffect(new MaterialEffectFloat3(bright));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(type, texture, renderer);

	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadHorizontalBlurPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, float blurGranularity, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("horizontal_blur", mRenderSystem->GetShader("HorizontalBlurShader"));
	material->AddOrReplaceEffect(new MaterialEffectFloat(blurGranularity));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(type, texture, renderer);
	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadVerticalBlurPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, float blurGranularity, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("vertical_blur", mRenderSystem->GetShader("VerticalBlurShader"));
	material->AddOrReplaceEffect(new MaterialEffectFloat(blurGranularity));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(type, texture,	renderer);
	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadBloomPostProcess(Model* model, Texture* texture, Texture* bright, PostProcessEffect::PostProcessEffectType type, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("bloom", mRenderSystem->GetShader("BloomShader"));
	material->AddOrReplaceEffect(new MaterialEffectDepthTexture(bright, 1.0f));

	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(type, texture, renderer);
	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::Load()
{
	ReleaseEffects();

	unsigned int screenWidth = static_cast<unsigned int>(mRenderSystem->GetScreenWidth());
	unsigned int screenHeight = static_cast<unsigned int>(mRenderSystem->GetScreenHeight());

	Texture* originalTexture = static_cast<Texture*>(mRenderSystem->CreateColorTexture("original", glm::vec2(screenWidth, screenHeight)));
	Texture* texture2 = static_cast<Texture*>(mRenderSystem->CreateColorTexture("postprocess2", glm::vec2(screenWidth, screenHeight)));
	Texture* texture3 = static_cast<Texture*>(mRenderSystem->CreateColorTexture("postprocess3", glm::vec2(screenWidth, screenHeight)));
	
	Model* model = mRenderSystem->GetModel("quad");
	
	/*
	LoadBrightPostProcess(model, texture2, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER, BRIGHT_VECTOR, screenWidth, screenHeight);
	LoadHorizontalBlurPostProcess(model, texture3, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER, screenWidth / 3.0f, screenWidth, screenHeight);
	LoadVerticalBlurPostProcess(model, texture2, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER, screenHeight/ 3.0f, screenWidth, screenHeight);
	LoadBloomPostProcess(model, texture3, originalTexture, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER, screenWidth, screenHeight);
	*/
	LoadContrastPostProcess(model, texture2, PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_SCREEN, screenWidth, screenHeight);
	
	if (!model->IsBuilt())
	{
		//TODO esto se puede hacer con un material por defecto en lugar de este
		model->Build(mRenderSystem->GetVertexBufferManager(), mRenderSystem->GetMaterial("contrast"));
	}

	//creating frame buffer
	mFrameBuffer = new IFrameBuffer(screenWidth, screenHeight);
	mFrameBuffer->SetCopyBufferToTexture(originalTexture, 0, 0, originalTexture->GetWidth(), originalTexture->GetHeight());
}

void PostProcessSubSystem::Render()
{
	ITexture* texture = mFrameBuffer->CopyColorBuffer();

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
