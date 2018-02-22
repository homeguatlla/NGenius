#include "stdafx.h"
#include "ShadowsRenderPass.h"

#include "RenderSystem.h"
#include "../../Transformation.h"
#include "../../camera/OrthogonalCamera.h"
#include "../../camera/ICamera.h"

#include "../../renderers/IRenderer_.h"
#include "../../../renderer/IFrameBuffer.h"
#include "../../../renderer/RenderPass.h"
#include "../../textures/Texture.h"

ShadowsRenderPass::ShadowsRenderPass(RenderSystem* renderSystem, float screenWidth, float screenHeight) :
mRenderSystem(renderSystem),
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mShadowMapTexture(nullptr),
mIsShadowCastEnabled(false),
mIsInitialized(false)
{
}

ShadowsRenderPass::~ShadowsRenderPass()
{
}

void ShadowsRenderPass::Init(Texture* shadowTexture)
{
	if (mIsShadowCastEnabled)
	{
		assert(shadowTexture != nullptr);

		mShadowMapTexture = shadowTexture;
		mShadowCastCamera = CreateShadowCastCamera(mDirectionalLightDirection);
		mRenderPass = CreateShadowRenderPass();
		mRenderSystem->AddRenderPass(mRenderPass);
		mIsInitialized = true;
	}
}

void ShadowsRenderPass::UpdateShadowCastMatrix()
{
	mShadowMapMatrix = CalculateShadowMapMatrix(mShadowCastCamera);
}

void ShadowsRenderPass::SetCastingShadowsTarget(const glm::vec3& position)
{
	if (mIsShadowCastEnabled)
	{
		mShadowCastCamera->SetPosition(glm::normalize(mDirectionalLightDirection) + position);
		mShadowCastCamera->SetTarget(position);
		mShadowCastCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
		UpdateShadowCastMatrix();
	}
}

void ShadowsRenderPass::SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter)
{
	mDirectionalLightDirection = lightDirection;
	mPFCCounter = pfcCounter;
}

void ShadowsRenderPass::SetEnable(bool enable)
{
	mIsShadowCastEnabled = enable;
	UpdateState();
}

void ShadowsRenderPass::UpdateState()
{
	if (mIsInitialized)
	{
		if (!mIsShadowCastEnabled)
		{
			mRenderSystem->RemoveRenderPass(mRenderPass);
		}
		else
		{
			mRenderSystem->AddRenderPass(mRenderPass);
		}
		mShadowMapTexture->SetActive(mIsShadowCastEnabled);
	}
}

bool ShadowsRenderPass::IsEnabled() const
{
	return mIsShadowCastEnabled;
}

const glm::mat4 ShadowsRenderPass::GetShadowMapMatrix() const
{
	return mShadowMapMatrix;
}

const Texture* ShadowsRenderPass::GetShadowMapTexture() const
{
	return mShadowMapTexture;
}

int ShadowsRenderPass::GetShadowMapPFCCounter() const
{
	return mPFCCounter;
}

glm::mat4 ShadowsRenderPass::CalculateShadowMapMatrix(const ICamera* camera)
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

ICamera* ShadowsRenderPass::CreateShadowCastCamera(const glm::vec3& directionalLightDirection)
{
	ICamera* camera = new OrthogonalCamera(mScreenWidth * 0.01f, mScreenHeight * 0.01f, -10.0f, 20.0f);

	return camera;
}

RenderPass* ShadowsRenderPass::CreateShadowRenderPass()
{
	//SHADOW RENDER PASS
	//SHADOW
	IFrameBuffer* frameShadowBuffer = new IFrameBuffer(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight));
	frameShadowBuffer->SetDepthTextureAttachment(mShadowMapTexture);
	frameShadowBuffer->Init();

	RenderPass* shadowPass = new RenderPass(static_cast<ICamera*>(mShadowCastCamera), IRenderer_::LAYER_OTHER);
	shadowPass->SetFrameBufferOutput(frameShadowBuffer);

	//IMaterial* material = mRenderSystem->CreateMaterial("shadow", mRenderSystem->GetShader("default"));
	//shadowPass->SetMaterial(material);

	return shadowPass;
}