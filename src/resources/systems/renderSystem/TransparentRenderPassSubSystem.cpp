#include "stdafx.h"
#include "TransparentRenderPassSubSystem.h"
#include "RenderSystem.h"
#include "RenderPass.h"
#include "../../GameConstants.h"
#include "../../camera/OrthogonalCamera.h"
#include "../../renderers/IRenderer.h"


TransparentRenderPassSubSystem::TransparentRenderPassSubSystem(RenderSystem* renderSystem, ICamera* camera) :
	mRenderSystem(renderSystem),
	mCamera(camera),
	mRenderPass(nullptr),
	mIsEnabled(false),
	mIsInitialized(false)
{
}


TransparentRenderPassSubSystem::~TransparentRenderPassSubSystem()
{
}


void TransparentRenderPassSubSystem::Init()
{
	if (mIsEnabled)
	{	
		mRenderPass = CreateGUIRenderPass(mCamera);
		mRenderPass->EnableFog(true);
		mRenderSystem->AddRenderPass(mRenderPass);

		mIsInitialized = true;
	}
}

void TransparentRenderPassSubSystem::SetEnable(bool enable)
{
	mIsEnabled = enable;
}


RenderPass* TransparentRenderPassSubSystem::CreateGUIRenderPass(ICamera* camera)
{
	//RENDER PASS TRANSPARENT
	return new RenderPass(static_cast<ICamera*>(camera), IRenderer::LAYER_TRANSPARENT);
}