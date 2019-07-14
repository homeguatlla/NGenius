#include "stdafx.h"
#include "FreeModeState.h"
#include "../../resources/camera/PerspectiveCamera.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../resources/materials/effects/MaterialEffectText.h"
#include "../../resources/entities/Text.h"
#include "../../resources/font/FontType.h"

#include "../../NGenius.h"
#include "../../GameConstants.h"
#include "../../utils/Log.h"

#include <GLFW\glfw3.h>
#include <string>

const float FORWARD_SPEED = 0.3f;
const float ROLL_SPEED = 2.0f;
const float PITCH_SPEED = 2.0f;
const std::string TEXT("Free Mode");

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
	CreateFreeModeStateUI();
}

void FreeModeState::OnEnter(float deltaTime)
{
	mFreeCamera = CreateFreeCamera();
	mIsNormalModeKeyPressed = false;
	mEngine->RegisterAllEventsInputListener(this);
	mEngine->ChangeToCamera(mEngine->GetGameplayCamera()->GetName(), mEngine->GetFreeCamera()->GetName());
	mFreeModeText->SetEnabled(true);
}

void FreeModeState::OnExit(float deltaTime)
{
	mEngine->UnRegisterInputListener(this);
	mIsNormalModeKeyPressed = false;
	mFreeModeText->SetEnabled(false);
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

		freeCamera = DBG_NEW  PerspectiveCamera("free_camera", 45.0f, aspectRatio, NEAR_PLANE, FAR_PLANE);
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

void FreeModeState::CreateFreeModeStateUI()
{
	FontType* font = mEngine->GetFont("OCR A Extended");
	IMaterial* materialText = mEngine->GetMaterial(MaterialsLibrary::TEXT_MATERIAL_NAME);
	materialText->AddEffect(DBG_NEW MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	materialText->AddEffect(DBG_NEW MaterialEffectText(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		0.4f,
		0.1f, 
		0.0f,
		0.0f,
		glm::vec2(0.0f)));

	mFreeModeText = DBG_NEW Text(
		DBG_NEW Transformation(
			glm::vec3(-mEngine->GetScreenWidth() * 0.5f, mEngine->GetScreenHeight() * 0.5f, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.70f)
		),
		materialText, font,
		TEXT, false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);
	
	mEngine->AddEntity(mFreeModeText);
}