#include "stdafx.h"
#include "PostProcessSubSystem.h"
#include "../../postprocesseffects/PostProcessEffect.h"
#include "../renderSystem/RenderSystem.h"
#include "../../renderers/ImageRenderer.h"
#include "../../models/Model.h"
#include "../../textures/Texture.h"
#include "../../materials/IMaterial.h"
#include "../../materials/effects/MaterialEffectDiffuseTexture.h"
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
	//test
	IMaterial* material = mRenderSystem->CreateMaterial("contrast", mRenderSystem->GetShader("contrast"));
	
	Model* model = mRenderSystem->GetModel("quad");
	if (!model->IsBuilt())
	{
		model->Build(mRenderSystem->GetVertexBufferManager(), material);
	}

	Texture* texture = static_cast<Texture*>(mRenderSystem->GetTexture("postprocess"));
	ImageRenderer* renderer = new ImageRenderer(model, material);
	PostProcessEffect* postProcessEffect = new PostProcessEffect(	PostProcessEffect::PostProcessEffectType::POSTPROCESS_TO_COLOR_BUFFER,
																	texture,
																	renderer);
	/*
	postProcessEffect->SetBufferSize(	static_cast<unsigned int>(mRenderSystem->GetScreenWidth()),
										static_cast<unsigned int>(mRenderSystem->GetScreenHeight()));
	*/
	AddPostProcessEffect(postProcessEffect);
	//test
	
	mFrameBuffer = new IFrameBuffer(mRenderSystem->GetScreenWidth(), mRenderSystem->GetScreenHeight());
	mFrameBuffer->SetCopyBufferToTexture(texture, 0, 0, static_cast<unsigned int>(mRenderSystem->GetScreenWidth()),
											static_cast<unsigned int>(mRenderSystem->GetScreenHeight()));
	//mFrameBuffer->Init();
	
	for (PostProcessEffect* postProcessEffect : mEffects)
	{
		postProcessEffect->Init();
	}
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
