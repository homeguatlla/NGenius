#include "stdafx.h"
#include "WaterRenderPassSubSystem.h"
#include "RenderSystem.h"
#include "../../Transformation.h"
#include "../../camera/PerspectiveCamera.h"
#include "../../camera/ICamera.h"

#include "../../renderers/IRenderer.h"
#include "IFrameBuffer.h"
#include "RenderPass.h"
#include "../../textures/Texture.h"
#include "../../materials/IMaterial.h"
#include "../../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../textures/ITexture.h"

#include <GL/glew.h>
#include <glm/glm.hpp> 

static const float VIEW_ANGLE = 45.0f;
static const float FAR_PLANE = 1000.0f;
static const float NEAR_PLANE = 0.1f;

WaterRenderPassSubSystem::WaterRenderPassSubSystem(RenderSystem* renderSystem, float screenWidth, float screenHeight) :
mReflectionCamera(nullptr),
mRefractionCamera(nullptr),
mGameplayCamera(nullptr),
mRenderSystem(renderSystem),
mReflectionRenderPass(nullptr),
mRefractionRenderPass(nullptr),
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mIsWaterEnabled(false),
mIsInitialized(false)
{
}

WaterRenderPassSubSystem::~WaterRenderPassSubSystem()
{
}

void WaterRenderPassSubSystem::Init()
{
	if (mIsWaterEnabled)
	{
		mReflectionCamera = CreateReflectionCamera();
		mRenderSystem->AddCamera(mReflectionCamera);
		mRefractionCamera = CreateRefractionCamera();
		mRenderSystem->AddCamera(mRefractionCamera);
		mReflectionRenderPass = CreateReflectionRenderPass();
		mRefractionRenderPass = CreateRefractionRenderPass();
		mRenderSystem->AddRenderPass(mReflectionRenderPass);
		mRenderSystem->AddRenderPass(mRefractionRenderPass);
		mIsInitialized = true;
	}
}

void WaterRenderPassSubSystem::SetEnable(bool enable)
{
	mIsWaterEnabled = enable;
}

bool WaterRenderPassSubSystem::IsEnabled() const
{
	return mIsWaterEnabled;
}

void WaterRenderPassSubSystem::SetWaterParameters(const ICamera* gameplayCamera, float waterY)
{
	assert(gameplayCamera != nullptr);
	mGameplayCamera = gameplayCamera;
	mWaterY = waterY;
}

ICamera* WaterRenderPassSubSystem::CreateReflectionCamera()
{
	return new PerspectiveCamera("reflection_camera", VIEW_ANGLE, mScreenWidth / mScreenHeight, NEAR_PLANE, FAR_PLANE);
}

ICamera* WaterRenderPassSubSystem::CreateRefractionCamera()
{
	return new PerspectiveCamera("refraction_camera", VIEW_ANGLE, mScreenWidth / mScreenHeight, NEAR_PLANE, FAR_PLANE);
}

RenderPass* WaterRenderPassSubSystem::CreateRefractionRenderPass()
{
	//REFRACTION	
	ApplyRefractionCameras(mGameplayCamera, mRefractionCamera);
	
	Texture* refractionTexture = static_cast<Texture*>(mRenderSystem->CreateColorTexture("refraction_water", glm::vec2(320 * 2, 240 * 2)));
	Texture* refractionDepthTexture = static_cast<Texture*>(mRenderSystem->CreateDepthTexture("refraction_depth_water", glm::vec2(320 * 2, 240 * 2)));
	
	IFrameBuffer* frameRefractionBuffer = new IFrameBuffer(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight));
	frameRefractionBuffer->SetColorTextureAttachment(0, refractionTexture);
	frameRefractionBuffer->SetDepthTextureAttachment(refractionDepthTexture);
	frameRefractionBuffer->Init();

	RenderPass* refractionWaterPass = new RenderPass(static_cast<ICamera*>(mRefractionCamera), IRenderer::LAYER_TERRAIN | IRenderer::LAYER_OTHER);
	refractionWaterPass->SetFrameBufferOutput(frameRefractionBuffer);
	refractionWaterPass->EnableClipping(true);
	refractionWaterPass->SetClippingPlaneNumber(GL_CLIP_DISTANCE0);
	//we need to add +2.0 to the water height, because when applying distorsion to the texture can get points 
	//out the refraction area drawed, and these points are blue (the color of the water plane), getting 
	//a bit more of the height, cutting over the water, we are painting more space in the water plane and then
	//the distorsion is not affecting, otherwise the border of the water has a bit of blue color (the default color of the plane)
	refractionWaterPass->SetClippingPlane(glm::vec4(0.0f, -1.0f, 0.0f, mWaterY + 2.0f));
	refractionWaterPass->EnableFog(false);

	return refractionWaterPass;
}

RenderPass* WaterRenderPassSubSystem::CreateReflectionRenderPass()
{
	//WATER RENDER PASS	
	ApplyReflectionCameras(mWaterY, mGameplayCamera, mReflectionCamera);

	//REFLECTION
	Texture* reflectionTexture = static_cast<Texture*>(mRenderSystem->CreateColorTexture("reflection_water", glm::vec2(320 * 2, 240 * 2)));
	IFrameBuffer* frameReflectionBuffer = new IFrameBuffer(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight));
	frameReflectionBuffer->SetColorTextureAttachment(0, reflectionTexture);
	frameReflectionBuffer->SetDepthAttachment(reflectionTexture->GetWidth(), reflectionTexture->GetHeight());

	frameReflectionBuffer->Init();

	RenderPass* reflectionWaterPass = new RenderPass(static_cast<ICamera*>(mReflectionCamera), IRenderer::LAYER_TERRAIN | IRenderer::LAYER_OTHER | IRenderer::LAYER_PARTICLES);
	reflectionWaterPass->SetFrameBufferOutput(frameReflectionBuffer);
	reflectionWaterPass->EnableClipping(true);
	reflectionWaterPass->SetClippingPlaneNumber(GL_CLIP_DISTANCE0);
	reflectionWaterPass->SetClippingPlane(glm::vec4(0.0f, 1.0f, 0.0f, -mWaterY + 0.2));
	reflectionWaterPass->EnableFog(true);

	return reflectionWaterPass;
}

void WaterRenderPassSubSystem::ApplyReflectionCameras(float yReflectionPlane, const ICamera* camera, ICamera* cameraReflected)
{
	float distance = 2 * (camera->GetPosition().y - yReflectionPlane);
	glm::vec3 position = camera->GetPosition();
	position.y -= distance;
	cameraReflected->SetPosition(position);

	distance = 2 * (camera->GetTarget().y - yReflectionPlane);
	glm::vec3 target = camera->GetTarget();
	target.y -= distance;
	cameraReflected->SetTarget(target);
	cameraReflected->SetUp(camera->GetUp());
}

void WaterRenderPassSubSystem::ApplyRefractionCameras(const ICamera* camera, ICamera* cameraRefracted)
{
	cameraRefracted->SetPosition(camera->GetPosition());
	cameraRefracted->SetTarget(camera->GetTarget());
	cameraRefracted->SetUp(camera->GetUp());
}

void WaterRenderPassSubSystem::Update()
{
	if (mIsInitialized)
	{
		ApplyReflectionCameras(mWaterY, mGameplayCamera, mReflectionCamera);
		ApplyRefractionCameras(mGameplayCamera, mRefractionCamera);
	}
}
