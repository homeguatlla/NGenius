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
		//Water reflection and refraction passes need to be rendered first, just after the gameplay render pass where the
		//water will be rendered and reflection and refraction textures will be needed.
		mRenderSystem->AddOrReplaceRenderPassFirst(mReflectionRenderPass);
		mRenderSystem->AddOrReplaceRenderPassFirst(mRefractionRenderPass);		
		
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
	return DBG_NEW PerspectiveCamera("reflection_camera", VIEW_ANGLE, mScreenWidth / mScreenHeight, NEAR_PLANE, FAR_PLANE);
}

ICamera* WaterRenderPassSubSystem::CreateRefractionCamera()
{
	return DBG_NEW PerspectiveCamera("refraction_camera", VIEW_ANGLE, mScreenWidth / mScreenHeight, NEAR_PLANE, FAR_PLANE);
}

RenderPass* WaterRenderPassSubSystem::CreateRefractionRenderPass()
{
	//REFRACTION	
	ApplyRefractionCameras(mGameplayCamera, mRefractionCamera);
	
	glm::vec2 textureSize(mScreenWidth / 1, mScreenHeight / 1);
	Texture* refractionTexture = static_cast<Texture*>(mRenderSystem->CreateColorTexture("refraction_water", textureSize));
	Texture* refractionDepthTexture = static_cast<Texture*>(mRenderSystem->CreateDepthTexture("refraction_depth_water", textureSize));
	
	IFrameBuffer* frameRefractionBuffer = DBG_NEW IFrameBuffer(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight));
	frameRefractionBuffer->SetColorTextureAttachment(0, refractionTexture);
	frameRefractionBuffer->SetDepthTextureAttachment(refractionDepthTexture);
	frameRefractionBuffer->Init();

	RenderPass* refractionWaterPass = DBG_NEW RenderPass("water_refraction_render_pass", static_cast<ICamera*>(mRefractionCamera), IRenderer::LAYER_TERRAIN | IRenderer::LAYER_OTHER | IRenderer::LAYER_PARTICLES | IRenderer::LAYER_TRANSPARENT);
	refractionWaterPass->SetFrameBufferOutput(frameRefractionBuffer);
	refractionWaterPass->EnableClipping(true);
	refractionWaterPass->SetClippingPlaneNumber(GL_CLIP_DISTANCE0);
	refractionWaterPass->SetClippingPlane(glm::vec4(0.0f, -1.0f, 0.0f, mWaterY));
	refractionWaterPass->SetAcceptSpacePartitionOnly(true);

	return refractionWaterPass;
}

RenderPass* WaterRenderPassSubSystem::CreateReflectionRenderPass()
{
	//WATER RENDER PASS	
	ApplyReflectionCameras(mWaterY, mGameplayCamera, mReflectionCamera);

	//REFLECTION
	//The textureSize MUST be with the same aspect ratio than the camera if not, when rendering will be a gap in black color
	//if texture size is equal to the camera ressolution, everything will work perfect, if not a noticiable cut will appear
	//on the borders.
	glm::vec2 textureSize(mScreenWidth / 1, mScreenHeight / 1);
	Texture* reflectionTexture = static_cast<Texture*>(mRenderSystem->CreateColorTexture("reflection_water", textureSize));
	IFrameBuffer* frameReflectionBuffer = DBG_NEW IFrameBuffer(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight));
	frameReflectionBuffer->SetColorTextureAttachment(0, reflectionTexture);
	frameReflectionBuffer->SetDepthAttachment(reflectionTexture->GetWidth(), reflectionTexture->GetHeight());

	frameReflectionBuffer->Init();

	RenderPass* reflectionWaterPass = DBG_NEW RenderPass("water_reflection_render_pass", static_cast<ICamera*>(mReflectionCamera), IRenderer::LAYER_REFLEXION | IRenderer::LAYER_TERRAIN | IRenderer::LAYER_OTHER | IRenderer::LAYER_PARTICLES | IRenderer::LAYER_TRANSPARENT);
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
	//cameraRefracted->SetUp(cameraRefracted->GetUp());
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
