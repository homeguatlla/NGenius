#include "stdafx.h"
#include "ShadowsSystem.h"

#include "RenderSystem.h"
#include "../Transformation.h"
#include "../camera/OrthogonalCamera.h"
#include "../camera/ICamera.h"

#include "../../renderer/IRenderer.h"
#include "../../renderer/IFrameBuffer.h"
#include "../../renderer/RenderPass.h"


ShadowsSystem::ShadowsSystem(RenderSystem* renderSystem, float screenWidth, float screenHeight, const Texture* shadowTexture) :
mRenderSystem(renderSystem),
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mShadowMapTexture(shadowTexture),
mIsShadowCastEnabled(false)
{
}

ShadowsSystem::~ShadowsSystem()
{
}

void ShadowsSystem::Init()
{
	mShadowCastCamera = CreateShadowCastCamera(mDirectionalLightDirection);
	RenderPass* renderPass = CreateShadowRenderPass();
	mRenderSystem->AddRenderPass(renderPass);
}

void ShadowsSystem::UpdateShadowCastMatrix()
{
	mShadowMapMatrix = CalculateShadowMapMatrix(mShadowCastCamera);
}

void ShadowsSystem::SetCastingShadowsTarget(const glm::vec3& position)
{
	if (mIsShadowCastEnabled)
	{
		mShadowCastCamera->SetPosition(glm::normalize(mDirectionalLightDirection) + position);
		mShadowCastCamera->SetTarget(position);
		mShadowCastCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
		UpdateShadowCastMatrix();
	}
}

void ShadowsSystem::SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter)
{
	mDirectionalLightDirection = lightDirection;
	mPFCCounter = pfcCounter;
}

void ShadowsSystem::SetEnable(bool enable)
{
	mIsShadowCastEnabled = enable;
}

bool ShadowsSystem::IsEnabled() const
{
	return mIsShadowCastEnabled;
}

const glm::mat4 ShadowsSystem::GetShadowMapMatrix() const
{
	return mShadowMapMatrix;
}

const Texture* ShadowsSystem::GetShadowMapTexture() const
{
	return mShadowMapTexture;
}

int ShadowsSystem::GetShadowMapPFCCounter() const
{
	return mPFCCounter;
}

glm::mat4 ShadowsSystem::CalculateShadowMapMatrix(const ICamera* camera)
{
	Transformation transformation(camera->GetPosition(), glm::vec3(0.0f), glm::vec3(1.0f));
	glm::mat4& matrix = camera->GetProjectionMatrix() * const_cast<ICamera*>(camera)->GetViewMatrix();

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	return biasMatrix * matrix;
}

ICamera* ShadowsSystem::CreateShadowCastCamera(const glm::vec3& directionalLightDirection)
{
	ICamera* camera = new OrthogonalCamera(mScreenWidth * 0.01f, mScreenHeight * 0.01f, -10.0f, 20.0f);

	return camera;
}

RenderPass* ShadowsSystem::CreateShadowRenderPass()
{
	//SHADOW RENDER PASS
	//SHADOW
	IFrameBuffer* frameShadowBuffer = new IFrameBuffer(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight));
	frameShadowBuffer->SetDepthTextureAttachment(mShadowMapTexture);
	frameShadowBuffer->Init();

	RenderPass* shadowPass = new RenderPass(static_cast<ICamera*>(mShadowCastCamera), IRenderer::LAYER_OTHER);
	shadowPass->SetFrameBufferOutput(frameShadowBuffer);
	//shadowPass->SetShader(mEngine.GetShader("quad"));
	return shadowPass;
}