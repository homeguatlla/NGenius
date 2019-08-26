#include "stdafx.h"
#include "FreeModeOnlyCameraState.h"
#include "../../resources/camera/PerspectiveCamera.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../resources/materials/effects/MaterialEffectText.h"
#include "../../resources/entities/Text.h"
#include "../../resources/font/FontType.h"
#include "../../resources/renderers/IRenderer.h"

#include "../GameConstants.h"

#include "../../NGenius.h"
#include "../../GameConstants.h"
#include "../../utils/Log.h"

#include <GLFW\glfw3.h>
#include <string>

const float FORWARD_SPEED = 0.3f;
const float ROLL_SPEED = 2.0f;
const float PITCH_SPEED = 2.0f;
const float PAN_SPEED = 0.3f;

FreeModeOnlyCameraState::FreeModeOnlyCameraState() :
	mFreeCamera(nullptr),
	mPitch(0.0f),
	mLastPitch(0.0f),
	mRoll(0.0f),
	mLastRoll(0.0f),
	mForwardSpeed(0.0f),
	mPanSpeed(0.0f)
{
}

FreeModeOnlyCameraState::~FreeModeOnlyCameraState()
{
	mEngine->UnRegisterInputListener(this);
}

void FreeModeOnlyCameraState::OnInit()
{
	mEngine = GetContext()->GetEngine();
}

void FreeModeOnlyCameraState::OnEnter(float deltaTime)
{
	mPlayer = mEngine->GetGameEntity("player");
	if (mPlayer == nullptr)
	{
		Log(Log::LOG_WARNING) << "FreeModeState did'nt find -player- gameEntity to disable input control.\n";
	}
	mFreeCamera = CreateFreeCamera();
	SaveFreeCameraData();
	GetContext()->ResetFreeMode();
	mEngine->RegisterAllEventsInputListener(this);
	mEngine->ChangeToCamera(mEngine->GetGameplayCamera()->GetName(), mEngine->GetFreeCamera()->GetName());
	GetContext()->GetInformationText()->UpdateText(TEXT_FREE_MODE);
	mPlayer->SetEnabled(false);
}

void FreeModeOnlyCameraState::OnExit(float deltaTime)
{
	mEngine->UnRegisterInputListener(this);
	mPlayer->SetEnabled(true);
}

void FreeModeOnlyCameraState::OnReload()
{
	mFreeCamera = CreateFreeCamera();
	RecoverFreeCameraData();
	mPlayer = mEngine->GetGameEntity("player");
}

void FreeModeOnlyCameraState::SaveFreeCameraData()
{
	mSavedCameraPosition = mFreeCamera->GetPosition();
	mSavedCameraTarget = mFreeCamera->GetTarget();
}

void FreeModeOnlyCameraState::RecoverFreeCameraData()
{
	mFreeCamera->SetPosition(mSavedCameraPosition);
	mFreeCamera->SetTarget(mSavedCameraTarget);
}

void FreeModeOnlyCameraState::OnUpdate(float deltaTime)
{
	CalculateCameraOrientation(deltaTime);
	CalculateCameraPosition(deltaTime);
	SaveFreeCameraData();
}

ICamera* FreeModeOnlyCameraState::CreateFreeCamera()
{
	ICamera* freeCamera = mEngine->GetCamera("free_camera");
	if (freeCamera == nullptr)
	{
		float screenWidth = static_cast<float>(mEngine->GetScreenWidth());
		float screenHeight = static_cast<float>(mEngine->GetScreenHeight());
		float aspectRatio = screenWidth / screenHeight;

		freeCamera = DBG_NEW  PerspectiveCamera("free_camera", 45.0f, aspectRatio, NEAR_PLANE, FAR_PLANE);
		freeCamera->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
		freeCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
		freeCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
		mEngine->AddCamera(freeCamera);
	}
	return freeCamera;
}

void FreeModeOnlyCameraState::CalculateCameraOrientation(float deltaTime)
{
	float currentRoll = glm::radians(mRoll * deltaTime * ROLL_SPEED);
	float currentPitch = glm::radians(mPitch * deltaTime * PITCH_SPEED);

	mFreeCamera->Rotate(currentRoll, mFreeCamera->GetUp());
	
	glm::vec3 direction = glm::normalize(mFreeCamera->GetTarget() - mFreeCamera->GetPosition());
	glm::vec3 orthogonal = glm::cross(mFreeCamera->GetUp(), direction);
	mFreeCamera->Rotate(currentPitch, -orthogonal);

	//Forcing the up vector in Y the roll and pitch are always related to Y vertical. 
	//if not, roll and pitch will move related a Up vector that can be different of Y vertical and
	//is more difficult to move the free camera.
	mFreeCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	mRoll = 0.0f;
	mPitch = 0.0f;
}

void FreeModeOnlyCameraState::CalculateCameraPosition(float deltaTime)
{
	if (mForwardSpeed != 0.0f)
	{
		mFreeCamera->Move(mForwardSpeed);
	}
	if (mPanSpeed != 0.0f)
	{
		mFreeCamera->Pan(mPanSpeed);
	}
	mForwardSpeed = 0.0f;
	mPanSpeed = 0.0f;
}

void FreeModeOnlyCameraState::OnKey(int key, int action)
{
	GetContext()->SetEnableFreeModeCamera(key == GLFW_KEY_F10 && action == GLFW_RELEASE);
	GetContext()->SetEnableFreeModePlayer(key == GLFW_KEY_F11 && action == GLFW_RELEASE);

	if (key == GLFW_KEY_W)
	{
		mForwardSpeed = glm::min(glm::exp(mForwardSpeed), FORWARD_SPEED);
	}
	else if (key == GLFW_KEY_S)
	{
		mForwardSpeed = -glm::min(glm::exp(mForwardSpeed), FORWARD_SPEED);
	}
	else
	{
		mForwardSpeed = 0.0f;
	}

	if (key == GLFW_KEY_A)
	{
		mPanSpeed = -PAN_SPEED;
	}
	else if (key == GLFW_KEY_D)
	{
		mPanSpeed = PAN_SPEED;
	}
	else 
	{
		mPanSpeed = 0.0f;
	}
}

void FreeModeOnlyCameraState::OnMouseScroll(int button, float scroll)
{
}

void FreeModeOnlyCameraState::OnMouseButton(int button, int action, int mods)
{
}

void FreeModeOnlyCameraState::OnMouseCursorPos(double x, double y)
{
	mPitch = mLastPitch - static_cast<float>(y);
	mRoll = mLastRoll - static_cast<float>(x);
	mLastPitch = static_cast<float>(y);
	mLastRoll = static_cast<float>(x);
}

