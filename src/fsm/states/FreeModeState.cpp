#include "stdafx.h"
#include "FreeModeState.h"
#include "../../resources/camera/PerspectiveCamera.h"
#include "../../NGenius.h"
#include "../../GameConstants.h"

void FreeModeState::OnInit()
{
	mEngine = GetContext()->GetEngine();
	CreateFreeCamera();
}

void FreeModeState::OnEnter(float deltaTime)
{
	mEngine->ChangeToCamera(std::string("gameplay_render_pass"), mFreeCamera);
}

void FreeModeState::OnExit(float deltaTime)
{
}

void FreeModeState::OnUpdate(float deltaTime)
{
}

void FreeModeState::CreateFreeCamera()
{
	mFreeCamera = mEngine->GetCamera("free_camera");
	if (mFreeCamera == nullptr)
	{
		float screenWidth = static_cast<float>(mEngine->GetScreenWidth());
		float screenHeight = static_cast<float>(mEngine->GetScreenHeight());
		float aspectRatio = screenWidth / screenHeight;

		mFreeCamera = new PerspectiveCamera("free_camera", 45.0f, aspectRatio, NEAR_PLANE, FAR_PLANE);
		mFreeCamera->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
		mFreeCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
		mFreeCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	}
}