#include "stdafx.h"
#include "ShadowsRenderPassSubSystem.h"

#include "RenderSystem.h"
#include "../../IGameEntity.h"
#include "../../camera/OrthogonalCamera.h"
#include "../../camera/ICamera.h"

#include "../../renderers/IRenderer.h"
#include "IFrameBuffer.h"
#include "RenderPass.h"
#include "../../textures/Texture.h"
#include "../../materials/IMaterial.h"
#include "../../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../materials/effects/MaterialEffectMatrix4Array.h"
#include "../../textures/ITexture.h"
#include "../../textures/Texture.h"
#include "../../shaders/ShadersLibrary.h"
#include "../../../utils/serializer/XMLDeserializer.h"

static const int SHADOWS_TEXTURE_SIZE = 2048;

ShadowsRenderPassSubSystem::ShadowsRenderPassSubSystem(RenderSystem* renderSystem, float screenWidth, float screenHeight) :
mRenderSystem(renderSystem),
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mShadowMapTexture(nullptr),
mTarget(nullptr),
mIsShadowCastEnabled(false),
mIsInitialized(false)
{
}

ShadowsRenderPassSubSystem::~ShadowsRenderPassSubSystem()
{
}

void ShadowsRenderPassSubSystem::Init()
{
	if (mIsShadowCastEnabled)
	{
		mShadowMapTexture = static_cast<Texture*>(mRenderSystem->CreateDepthTexture("shadow_texture", glm::ivec2(SHADOWS_TEXTURE_SIZE)));
		assert(mShadowMapTexture != nullptr);

		mShadowCastCamera = CreateShadowCastCamera(mDirectionalLightDirection);
		mRenderSystem->AddCamera(mShadowCastCamera);
		mRenderPass = CreateShadowRenderPass();
		mRenderSystem->AddOrReplaceRenderPass(mRenderPass);
		mIsInitialized = true;
	}
}

void ShadowsRenderPassSubSystem::Update()
{
	if (mIsInitialized && mIsShadowCastEnabled && HasTarget())
	{
		UpdateShadowCastMatrix();
	}
}

void ShadowsRenderPassSubSystem::UpdateShadowCastMatrix()
{
	glm::vec3 position = mTarget->GetTransformation()->GetPosition();
	mShadowCastCamera->SetPosition(glm::normalize(mDirectionalLightDirection) + position);
	mShadowCastCamera->SetTarget(position);
	mShadowCastCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	mShadowMapMatrix = CalculateShadowMapMatrix(mShadowCastCamera);
}

void ShadowsRenderPassSubSystem::ReadFrom(core::utils::IDeserializer* source)
{
	source->BeginAttribute("shadows");
		source->ReadParameter("is_enabled", &mIsShadowCastEnabled);
	source->EndAttribute();
}

void ShadowsRenderPassSubSystem::WriteTo(core::utils::ISerializer *destination)
{
}

void ShadowsRenderPassSubSystem::SetCastingShadowsTarget(const IGameEntity* target)
{
	mTarget = target;
}

void ShadowsRenderPassSubSystem::SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter)
{
	mDirectionalLightDirection = lightDirection;
	mPFCCounter = pfcCounter;
}

void ShadowsRenderPassSubSystem::SetEnable(bool enable)
{
	mIsShadowCastEnabled = enable;
	UpdateState();
}

void ShadowsRenderPassSubSystem::UpdateState()
{
	if (mIsInitialized)
	{
		if (!mIsShadowCastEnabled)
		{
			mRenderSystem->RemoveRenderPass(mRenderPass);
		}
		else
		{
			mRenderSystem->AddOrReplaceRenderPass(mRenderPass);
		}
		mShadowMapTexture->SetActive(mIsShadowCastEnabled);
	}
}

bool ShadowsRenderPassSubSystem::IsEnabled() const
{
	return mIsShadowCastEnabled;
}

const glm::mat4 ShadowsRenderPassSubSystem::GetShadowMapMatrix() const
{
	return mShadowMapMatrix;
}

const Texture* ShadowsRenderPassSubSystem::GetShadowMapTexture() const
{
	return mShadowMapTexture;
}

int ShadowsRenderPassSubSystem::GetShadowMapPFCCounter() const
{
	return mPFCCounter;
}

glm::mat4 ShadowsRenderPassSubSystem::CalculateShadowMapMatrix(const ICamera* camera)
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

ICamera* ShadowsRenderPassSubSystem::CreateShadowCastCamera(const glm::vec3& directionalLightDirection)
{
	ICamera* camera = DBG_NEW OrthogonalCamera("shadow_camera", mScreenWidth * 0.01f, mScreenHeight * 0.01f, -10.0f, 20.0f);

	return camera;
}

RenderPass* ShadowsRenderPassSubSystem::CreateShadowRenderPass()
{
	//SHADOW RENDER PASS
	//SHADOW
	IFrameBuffer* frameShadowBuffer = DBG_NEW IFrameBuffer(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight));

	//Texture* colorTexture = static_cast<Texture*>(mRenderSystem->CreateColorTexture("shadow_color", glm::ivec2(SHADOWS_TEXTURE_SIZE)));

	//frameShadowBuffer->SetColorTextureAttachment(0, colorTexture);
	frameShadowBuffer->SetDepthTextureAttachment(mShadowMapTexture);
	frameShadowBuffer->Init();

	RenderPass* shadowPass = DBG_NEW RenderPass("shadows_render_pass", static_cast<ICamera*>(mShadowCastCamera), IRenderer::LAYER_OTHER);
	shadowPass->SetFrameBufferOutput(frameShadowBuffer);

	IMaterial* material = mRenderSystem->CreateMaterial("shadow", mRenderSystem->GetShader(ShadersLibrary::SHADOW_SHADER_NAME));
	material->AddEffect(new MaterialEffectDiffuseTexture(mShadowMapTexture, glm::vec3(1.0f), 1.0f));
	//for animated models
	material->AddEffect(new MaterialEffectMatrix4Array());

	shadowPass->SetMaterial(material);

	return shadowPass;
}