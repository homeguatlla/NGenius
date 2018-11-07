#include "stdafx.h"
#include "GUIRenderPassSubSystem.h"
#include "RenderSystem.h"
#include "RenderPass.h"
#include "../../GameConstants.h"
#include "../../camera/OrthogonalCamera.h"
#include "../../renderers/IRenderer.h"


GUIRenderPassSubSystem::GUIRenderPassSubSystem(RenderSystem* renderSystem, float screenWidth, float screenHeight) :
	mRenderSystem(renderSystem),
	mCamera(nullptr),
	mRenderPass(nullptr),
	mScreenWidth(screenWidth),
	mScreenHeight(screenHeight),
	mIsEnabled(false),
	mIsInitialized(false)
{
}


GUIRenderPassSubSystem::~GUIRenderPassSubSystem()
{
}


void GUIRenderPassSubSystem::Init()
{
	if (mIsEnabled)
	{	
		mCamera = CreateCamera();
		mRenderSystem->AddCamera(mCamera);

		mRenderPass = CreateGUIRenderPass(mCamera);
		mRenderSystem->AddRenderPass(mRenderPass);

		mIsInitialized = true;
	}
}

void GUIRenderPassSubSystem::SetEnable(bool enable)
{
	mIsEnabled = enable;
}

ICamera* GUIRenderPassSubSystem::CreateCamera()
{
	ICamera* camera = new OrthogonalCamera("gui_camera", mScreenWidth, mScreenHeight, NEAR_PLANE, FAR_PLANE);
	camera->SetPosition(glm::vec3(0.0f, 0.0f, 40.0f));
	camera->SetTarget(glm::vec3(0.0f, 0.0f, -50.0f));
	camera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));

	return camera;
}

RenderPass* GUIRenderPassSubSystem::CreateGUIRenderPass(ICamera* camera)
{
	//RENDER PASS GUI
	return new RenderPass(static_cast<ICamera*>(camera), IRenderer::LAYER_GUI);
}