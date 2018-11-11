#include "stdafx.h"
#include "ParticlesRenderPassSubSystem.h"
#include "RenderSystem.h"
#include "RenderPass.h"
#include "../../GameConstants.h"
#include "../../camera/OrthogonalCamera.h"
#include "../../renderers/IRenderer.h"


ParticlesRenderPassSubSystem::ParticlesRenderPassSubSystem(RenderSystem* renderSystem, ICamera* camera) :
	mRenderSystem(renderSystem),
	mCamera(camera),
	mRenderPass(nullptr),
	mIsEnabled(false),
	mIsInitialized(false)
{
}


ParticlesRenderPassSubSystem::~ParticlesRenderPassSubSystem()
{
}


void ParticlesRenderPassSubSystem::Init()
{
	if (mIsEnabled)
	{	
		mRenderPass = CreateRenderPass(mCamera);
		mRenderPass->SetCalculateDistanceToCamera(true);
		mRenderPass->EnableFog(true);
		mRenderSystem->AddRenderPass(mRenderPass);

		mIsInitialized = true;
	}
}

void ParticlesRenderPassSubSystem::SetEnable(bool enable)
{
	mIsEnabled = enable;
}

RenderPass* ParticlesRenderPassSubSystem::CreateRenderPass(ICamera* camera)
{
	//RENDER PASS Particles
	return new RenderPass(static_cast<ICamera*>(camera), IRenderer::LAYER_PARTICLES);
}