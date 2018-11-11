#include "stdafx.h"
#include "GameplayRenderPassSubSystem.h"
#include "RenderSystem.h"
#include "RenderPass.h"
#include "../../GameConstants.h"
#include "../../camera/OrthogonalCamera.h"
#include "../../renderers/IRenderer.h"
#include "../../systems/renderSystem/IFrameBuffer.h"
#include "../../textures/Texture.h"


GameplayRenderPassSubSystem::GameplayRenderPassSubSystem(RenderSystem* renderSystem, ICamera* camera) :
	mRenderSystem(renderSystem),
	mCamera(camera),
	mRenderPass(nullptr),
	mIsEnabled(false),
	mIsInitialized(false)
{
}


GameplayRenderPassSubSystem::~GameplayRenderPassSubSystem()
{
}


void GameplayRenderPassSubSystem::Init()
{
	if (mIsEnabled)
	{	
		mFrameBuffer = CreateFrameBuffer();

		mRenderPass = CreateGUIRenderPass(mCamera);
		mRenderPass->SetAcceptSpacePartitionOnly(true);
		mRenderPass->SetFrameBufferOutput(mFrameBuffer);
		mRenderPass->EnableFog(true);

		mRenderSystem->AddRenderPass(mRenderPass);

		mIsInitialized = true;
	}
}

void GameplayRenderPassSubSystem::SetEnable(bool enable)
{
	mIsEnabled = enable;
}

RenderPass* GameplayRenderPassSubSystem::CreateGUIRenderPass(ICamera* camera)
{
	//RENDER PASS GAMEPLAY
	return new RenderPass(static_cast<ICamera*>(camera), IRenderer::LAYER_OTHER | IRenderer::LAYER_WATER | IRenderer::LAYER_DEBUG);
}

IFrameBuffer* GameplayRenderPassSubSystem::CreateFrameBuffer()
{
	float screenWidth = mRenderSystem->GetScreenWidth();
	float screenHeight = mRenderSystem->GetScreenHeight();
	Texture* depthTexture = static_cast<Texture*>(mRenderSystem->CreateDepthTexture("depth_texture", glm::vec2(screenWidth, screenHeight)));

	IFrameBuffer* frameBuffer = new IFrameBuffer(screenWidth, screenHeight);
	frameBuffer->SetCopyBufferToTexture(depthTexture, 0, 0, screenWidth, screenHeight);
	
	return frameBuffer;
}