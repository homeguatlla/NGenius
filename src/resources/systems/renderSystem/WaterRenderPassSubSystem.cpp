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
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../../utils/Log.h"

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
		ICamera* camera = mRenderSystem->GetCamera(mCameraName);
		if (camera != nullptr)
		{
			SetWaterParameters(camera, mWaterY);
		}
		else
		{
			Log(Log::LOG_ERROR) << "Could not find the water camera " << mCameraName << "\n";
		}
		mReflectionCamera = CreateReflectionCamera();
		mRenderSystem->AddCamera(mReflectionCamera);
		mRefractionCamera = CreateRefractionCamera();
		mRenderSystem->AddCamera(mRefractionCamera);
		mReflectionRenderPass = CreateReflectionRenderPass();
		mRefractionRenderPass = CreateRefractionRenderPass();
		//OJO! cuando se crean esto renderpass, por algún motivo no pueden terminar teniendo el mismo layer id
		//lo que quiere decir que tienen que tener alguna layer_mask distinta como ahora
		//que la reflexion tiene la skybox y la refracción no.
		//sino, una de las dos renderpass no se renderizará bien, o la refracción o la relflexión. No sé el motivo.
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

float WaterRenderPassSubSystem::GetWaterHeight() const
{
	return mWaterY;
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

	RenderPass* refractionWaterPass = new RenderPass("water_refraction_render_pass", static_cast<ICamera*>(mRefractionCamera), IRenderer::LAYER_TERRAIN | IRenderer::LAYER_OTHER | IRenderer::LAYER_PARTICLES | IRenderer::LAYER_TRANSPARENT);
	refractionWaterPass->SetFrameBufferOutput(frameRefractionBuffer);
	refractionWaterPass->EnableClipping(true);
	refractionWaterPass->SetClippingPlaneNumber(GL_CLIP_DISTANCE0);
	//we need to add +2.0 to the water height, because when applying distorsion to the texture can get points 
	//out the refraction area drawed, and these points are blue (the color of the water plane), getting 
	//a bit more of the height, cutting over the water, we are painting more space in the water plane and then
	//the distorsion is not affecting, otherwise the border of the water has a bit of blue color (the default color of the plane)
	refractionWaterPass->SetClippingPlane(glm::vec4(0.0f, -1.0f, 0.0f, mWaterY + 2.0f));
	refractionWaterPass->SetAcceptSpacePartitionOnly(true);

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

	RenderPass* reflectionWaterPass = new RenderPass("water_reflection_render_pass", static_cast<ICamera*>(mReflectionCamera), IRenderer::LAYER_REFLEXION | IRenderer::LAYER_TERRAIN | IRenderer::LAYER_OTHER | IRenderer::LAYER_PARTICLES | IRenderer::LAYER_TRANSPARENT);
	reflectionWaterPass->SetFrameBufferOutput(frameReflectionBuffer);
	reflectionWaterPass->EnableClipping(true);
	reflectionWaterPass->SetClippingPlaneNumber(GL_CLIP_DISTANCE0);
	reflectionWaterPass->SetClippingPlane(glm::vec4(0.0f, 1.0f, 0.0f, -mWaterY /*+ 0.2*/)); //eliminamos este +0.2 pues hace que se vea un poco del reflejo de las cosas que están debajo del agua.
	reflectionWaterPass->EnableFog(true);
	reflectionWaterPass->SetAcceptSpacePartitionOnly(true);

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
	//camera reflected will calculate related a Y positive vector.
	cameraReflected->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

void WaterRenderPassSubSystem::ApplyRefractionCameras(const ICamera* camera, ICamera* cameraRefracted)
{
	cameraRefracted->SetPosition(camera->GetPosition());
	cameraRefracted->SetTarget(camera->GetTarget());
	//camera refracted will be calculated related an Y positive vector.
	cameraRefracted->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

void WaterRenderPassSubSystem::ReadFrom(core::utils::IDeserializer* source)
{
	source->BeginAttribute("water");
		source->ReadParameter("is_enabled", &mIsWaterEnabled);
		source->ReadParameter("camera", mCameraName);
		source->ReadParameter("height", &mWaterY);
	source->EndAttribute();	
}

void WaterRenderPassSubSystem::WriteTo(core::utils::ISerializer* destination)
{
}

void WaterRenderPassSubSystem::Update()
{
	if (mIsInitialized)
	{
		ApplyReflectionCameras(mWaterY, mGameplayCamera, mReflectionCamera);
		ApplyRefractionCameras(mGameplayCamera, mRefractionCamera);
	}
}
