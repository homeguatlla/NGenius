#include "stdafx.h"
#include "FreeModeState.h"
#include "../../resources/camera/PerspectiveCamera.h"
#include "../../NGenius.h"
#include "../../GameConstants.h"
#include "../../utils/Log.h"
#include <GLFW\glfw3.h>

const float FORWARD_SPEED = 0.3f;
const float ROLL_SPEED = 2.0f;
const float PITCH_SPEED = 2.0f;

FreeModeState::FreeModeState() : 
	mIsNormalModeKeyPressed(false),
	mPitch(0.0f),
	mLastPitch(0.0f),
	mRoll(0.0f),
	mLastRoll(0.0f),
	mForwardSpeed(0.0f)
{
}

FreeModeState::~FreeModeState()
{
	mEngine->UnRegisterInputListener(this);
}

void FreeModeState::OnInit()
{
	mEngine = GetContext()->GetEngine();
	mFreeCamera = CreateFreeCamera();
}

void FreeModeState::OnEnter(float deltaTime)
{
	mIsNormalModeKeyPressed = false;
	mEngine->RegisterAllEventsInputListener(this);
	mEngine->ChangeToCamera(mEngine->GetGameplayCamera()->GetName(), mEngine->GetFreeCamera()->GetName());
}

void FreeModeState::OnExit(float deltaTime)
{
	mEngine->UnRegisterInputListener(this);
	mIsNormalModeKeyPressed = false;
}

void FreeModeState::OnUpdate(float deltaTime)
{
	CalculateCameraOrientation(deltaTime);
	CalculateCameraPosition(deltaTime);
}

ICamera* FreeModeState::CreateFreeCamera()
{
	ICamera* freeCamera = mEngine->GetCamera("free_camera");
	if (freeCamera == nullptr)
	{
		float screenWidth = static_cast<float>(mEngine->GetScreenWidth());
		float screenHeight = static_cast<float>(mEngine->GetScreenHeight());
		float aspectRatio = screenWidth / screenHeight;

		freeCamera = new PerspectiveCamera("free_camera", 45.0f, aspectRatio, NEAR_PLANE, FAR_PLANE);
		freeCamera->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
		freeCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
		freeCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
		mEngine->AddCamera(freeCamera);
	}
	return freeCamera;
}

void FreeModeState::CalculateCameraOrientation(float deltaTime)
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

void FreeModeState::CalculateCameraPosition(float deltaTime)
{
	mFreeCamera->Move(mForwardSpeed);
	mForwardSpeed = 0.0f;
}

void FreeModeState::OnKey(int key, int action)
{
	mIsNormalModeKeyPressed = (key == GLFW_KEY_F10 && action == GLFW_RELEASE);
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
}

void FreeModeState::OnMouseScroll(int button, float scroll)
{
}

void FreeModeState::OnMouseButton(int button, int action, int mods)
{
}

void FreeModeState::OnMouseCursorPos(double x, double y)
{
	mPitch = mLastPitch - static_cast<float>(y);
	mRoll = mLastRoll - static_cast<float>(x);
	mLastPitch = static_cast<float>(y);
	mLastRoll = static_cast<float>(x);
}