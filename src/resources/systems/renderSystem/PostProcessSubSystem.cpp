#include "stdafx.h"
#include "PostProcessSubSystem.h"
#include "src/resources/systems/renderSystem/RenderSystem.h"
#include "src/resources/renderers/ImageRenderer.h"
#include "src/resources/models/Model.h"
#include "src/resources/textures/Texture.h"
#include "src/resources/materials/IMaterial.h"
#include "src/resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "src/resources/materials/effects/MaterialEffectDepthTexture.h"
#include "src/resources/materials/effects/MaterialEffectFloat.h"
#include "src/resources/materials/effects/MaterialEffectFloat3.h"
#include "src/resources/systems/renderSystem/IFrameBuffer.h"
#include "src/resources/models/ModelsLibrary.h"
#include "../Memory.h"

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

	ImageRenderer* renderer = DBG_NEW ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = DBG_NEW PostProcessEffect(type, texture,	renderer);

	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadBrightPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, const glm::vec3& bright, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("bright", mRenderSystem->GetShader("BrightShader"));
	material->AddOrReplaceEffect(new MaterialEffectFloat3(bright));

	ImageRenderer* renderer = DBG_NEW ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = DBG_NEW PostProcessEffect(type, texture, renderer);

	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadHorizontalBlurPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, float blurGranularity, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("horizontal_blur", mRenderSystem->GetShader("HorizontalBlurShader"));
	material->AddOrReplaceEffect(new MaterialEffectFloat(blurGranularity));

	ImageRenderer* renderer = DBG_NEW ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = DBG_NEW PostProcessEffect(type, texture, renderer);
	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadVerticalBlurPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, float blurGranularity, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("vertical_blur", mRenderSystem->GetShader("VerticalBlurShader"));
	material->AddOrReplaceEffect(new MaterialEffectFloat(blurGranularity));

	ImageRenderer* renderer = DBG_NEW ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = DBG_NEW PostProcessEffect(type, texture,	renderer);
	postProcessEffect->SetBufferSize(width, height);
	AddPostProcessEffect(postProcessEffect);
}

void PostProcessSubSystem::LoadBloomPostProcess(Model* model, Texture* texture, Texture* bright, PostProcessEffect::PostProcessEffectType type, unsigned int width, unsigned int height)
{
	IMaterial* material = mRenderSystem->CreateMaterial("bloom", mRenderSystem->GetShader("BloomShader"));
	material->AddOrReplaceEffect(new MaterialEffectDepthTexture(bright, 1.0f));

	ImageRenderer* renderer = DBG_NEW ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = DBG_NEW PostProcessEffect(type, texture, renderer);
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
	//Texture* texture3 = static_cast<Texture*>(mRenderSystem->CreateColorTexture("postprocess3", glm::vec2(screenWidth, screenHeight)));
	

	originalTexture->SetUnit(GL_TEXTURE21);
	texture2->SetUnit(GL_TEXTURE22);

	Model* model = mRenderSystem->GetModel(ModelsLibrary::QUAD_NAME);
	
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
	mFrameBuffer = DBG_NEW IFrameBuffer(screenWidth, screenHeight);
	mFrameBuffer->SetCopyBufferToTexture(originalTexture, 0, 0, originalTexture->GetWidth(), originalTexture->GetHeight());
}

void PostProcessSubSystem::Render()
{
	ITexture* texture = mFrameBuffer->CopyColorBuffer();

	mRenderSystem->ApplyDepthTest(false, false, GL_LESS);
	for (PostProcessEffect* postProcessEffect : mEffects)
	{
		texture = postProcessEffect->Render(mRenderSystem, texture);
	}
	mRenderSystem->ApplyDepthTest(true, true, GL_LESS);
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
