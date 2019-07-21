#include "stdafx.h"
#include "ThirdPersonCameraComponent.h"

#include <glm/glm.hpp>
#include "../IGameEntity.h"
#include "../camera/PerspectiveCamera.h"
#include "../Transformation.h"
#include "../GameEvent.h"
#include "../events/characterControllerEvents/ZoomEvent.h"
#include "../events/characterControllerEvents/PitchEvent.h"
#include "CollisionComponent.h"
#include "OverWaterComponent.h"
#include "CharacterComponent.h"

#include "../../utils/serializer/XMLSerializer.h"
#include <iostream>

//WARNING!! hay que tener en cuenta que, si subimos este valor la cámara por colisión subirá y no mantendrá el ángulo pitch que le hemos definido.
//es decir, si vemos que el ángulo de la cámara con el target es demasiado alto (vemos al player desde una posición más alta) hay que tener en cuenta que haya subido por este valor
const float CAMERA_HEIGHT_OFFSET_GROUND = 0.2f;
const float CAMERA_SMOOTH_MOVEMENT_VALUE = 0.2f;
const float WATER_HEIGHT_OFFSET = 0.1f;

const float MAX_PITCH = 89.0f;//degrees
const float MIN_PITCH = 5.0f;
//this hysteresis if to make camera don't follow target when distance between camera and target < mDistanceToTarget-HYSTERESIS and 
//start following again if distance > mDistance+HYSTERESIS, this way the player can move with freedom of the camera near it.
const float HYSTERESIS = 0.2f;
const float MIN_DISTANCE_TO_START_FOLLOW = 2.0f;
const float MAX_ZOOM = 50.0f;
const float MIN_ZOOM = 2.0f; // MIN_DISTANCE_TO_START_FOLLOW;

ThirdPersonCameraComponent::ThirdPersonCameraComponent() : mZoomSpeed(1.0f)
{
}


ThirdPersonCameraComponent::ThirdPersonCameraComponent(PerspectiveCamera* camera, IGameEntity* target, const glm::vec3& targetOffset, float distanceFromTarget, float pitch, float pitchSpeed, float zoomSpeed) :
	mCamera(camera), 
	mTarget(target), 
	mTargetOffset(targetOffset),
	mDistanceFromTarget(distanceFromTarget), 
	mPitch(pitch),
	mPitchSpeed(pitchSpeed),
	mCurrentPitch(pitch),
	mLastPitch(pitch),
	mAngleAroundTarget(0.0f), 
	mZoomSpeed(zoomSpeed),
	mIsCameraFollowingTarget(true)
{
	assert(target != nullptr);
}

ThirdPersonCameraComponent::~ThirdPersonCameraComponent()
{
}

ThirdPersonCameraComponent* ThirdPersonCameraComponent::DoClone() const
{
	return DBG_NEW ThirdPersonCameraComponent(*this);
}

void ThirdPersonCameraComponent::UpdateInternal(float elapsedTime)
{
	UpdateGameEvents(elapsedTime);

	glm::vec3 newTarget = mTarget->GetTransformation()->GetPosition() + mTargetOffset;
	glm::vec3 currentTarget = mCamera->GetTarget();
	newTarget = currentTarget + (newTarget - currentTarget) * CAMERA_SMOOTH_MOVEMENT_VALUE;

	mCamera->SetTarget(newTarget);
	glm::vec3 currentPosition = mCamera->GetPosition();
	glm::vec3 newPosition = GetCameraPosition();

	if (mParent->HasComponent<CollisionComponent>())
	{
		CollisionComponent* collisionComponent = mParent->GetComponent<CollisionComponent>();
		float groundHeight = collisionComponent->GetGroundHeight() + CAMERA_HEIGHT_OFFSET_GROUND;
		if (newPosition.y < groundHeight)
		{
			newPosition.y = groundHeight;
		}
	}

	if (mParent->HasComponent<OverWaterComponent>())
	{
		OverWaterComponent* overWaterComponent = mParent->GetComponent<OverWaterComponent>();
		float waterHeight = overWaterComponent->GetWaterHeight() + WATER_HEIGHT_OFFSET;
		if (newPosition.y < waterHeight)
		{
			newPosition.y = waterHeight;
		}
	}

	newPosition = currentPosition + (newPosition - currentPosition) * CAMERA_SMOOTH_MOVEMENT_VALUE;

	//la camara deja de seguir al target si está por debajo de la distancia target-camera y 
	//vuelve a seguir al target si está por encima
	/*float distanceTargetCamera = glm::distance(newPosition, mTarget->GetTransformation()->GetPosition());
	if (!mIsCameraFollowingTarget && distanceTargetCamera > MIN_DISTANCE_TO_START_FOLLOW + HYSTERESIS)
	{
		mIsCameraFollowingTarget = true;
		//std::cout << "distance = " << distanceTargetCamera << "ON" << "\n";

	}
	else if(mIsCameraFollowingTarget && distanceTargetCamera < MIN_DISTANCE_TO_START_FOLLOW - HYSTERESIS)
	{
		mIsCameraFollowingTarget = false;
		//std::cout << "distance = " << distanceTargetCamera << "OFF" << "\n";
	}
	if (mIsCameraFollowingTarget)
	{
		mCamera->SetPosition(newPosition);
	}*/
	mCamera->SetPosition(newPosition);
	mParent->GetTransformation()->SetPosition(newPosition);
}

void ThirdPersonCameraComponent::UpdateGameEvents(float elapsedTime)
{
	CharacterComponent* characterComponent = mParent->GetComponent<CharacterComponent>();
	while (characterComponent->HasEvents())
	{
		const GameEvent* event = characterComponent->ConsumeEvent();
		if (event->IsOfType<ZoomEvent>())
		{
			const ZoomEvent* zoomEvent = static_cast<const ZoomEvent*>(event);
			UpdateZoom(zoomEvent->GetZoom(), elapsedTime);
		}
		else if (event->IsOfType<PitchEvent>())
		{
			const PitchEvent* pitchEvent = static_cast<const PitchEvent*>(event);
			UpdatePitch(pitchEvent->GetPitch(), elapsedTime);
		}
	}
}

const IGameEntity* ThirdPersonCameraComponent::GetTarget() const
{
	return mTarget;
}

glm::vec3 ThirdPersonCameraComponent::GetCameraPosition() const
{
	float horizontalDistance = CalculateHorizontalDistance();
	float verticalDistance = CalculateVerticalDistance();

	glm::vec3 newPosition = CalculateCameraPosition(horizontalDistance, verticalDistance);

	//yaw = 180 - (mAngleAroundTarget + std::degrees(mTarget->GetTransformation()->GetRotation().y))
	return newPosition;
}

float ThirdPersonCameraComponent::GetCameraPitch() const
{
	return mCurrentPitch;
}

IComponent* ThirdPersonCameraComponent::Create(IGameEntity* entity)
{
	ThirdPersonCameraComponent* component = DBG_NEW ThirdPersonCameraComponent();
	entity->AddComponent(component);

	return component;
}

float ThirdPersonCameraComponent::CalculateHorizontalDistance() const
{
	/*glm::vec3 currentPosition = mCamera->GetPosition();
	float distance = glm::distance(mTarget->GetTransformation()->GetPosition(), currentPosition);
	distance = glm::min(distance, mDistanceFromTarget);
	*/
	return mDistanceFromTarget * glm::cos(glm::radians(mCurrentPitch));
}

float ThirdPersonCameraComponent::CalculateVerticalDistance() const
{
	/*glm::vec3 currentPosition = mCamera->GetPosition();
	float distance = glm::distance(mTarget->GetTransformation()->GetPosition(), currentPosition);
	distance = glm::min(distance, mDistanceFromTarget);
	*/
	return mDistanceFromTarget * glm::sin(glm::radians(mCurrentPitch));
}

glm::vec3 ThirdPersonCameraComponent::CalculateCameraPosition(float horizontalDistance, float verticalDistance) const
{
	glm::vec3 targetPosition = mTarget->GetTransformation()->GetPosition();
	float theta = mAngleAroundTarget + mTarget->GetTransformation()->GetRotation().y;
	float offsetX = horizontalDistance * glm::sin(theta);
	float offsetZ = horizontalDistance * glm::cos(theta);

	glm::vec3 newPosition;
	newPosition.x = targetPosition.x - offsetX;
	newPosition.y = targetPosition.y + verticalDistance;
	newPosition.z = targetPosition.z - offsetZ;

	return newPosition;
}

void ThirdPersonCameraComponent::UpdateZoom(float scroll, float elapsedTime)
{
	float currentZoomSpeed = mZoomSpeed * scroll * elapsedTime;
	mDistanceFromTarget += currentZoomSpeed;
	mDistanceFromTarget = glm::max(MIN_ZOOM, glm::min(MAX_ZOOM, mDistanceFromTarget));
}

void ThirdPersonCameraComponent::UpdatePitch(float pitch, float elapsedTime)
{
	float pitchSpeed = (pitch - mLastPitch) * mPitchSpeed * elapsedTime;
	mCurrentPitch += pitchSpeed;
	mCurrentPitch = glm::max(MIN_PITCH, glm::min(MAX_PITCH, mCurrentPitch));
	mLastPitch = pitch;
}

void ThirdPersonCameraComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void ThirdPersonCameraComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("third_person_camera_component"));
	destination->WriteParameter(std::string("camera_name"), mCamera->GetName());
	destination->WriteParameter(std::string("target_entity_id"), mTarget->GetID(), "");
	destination->WriteParameter(std::string("target_offset"), mTargetOffset);
	destination->WriteParameter(std::string("distance_from_target"), mDistanceFromTarget);
	destination->WriteParameter(std::string("pitch"), mPitch);
	destination->WriteParameter(std::string("pitch_speed"), mPitchSpeed);
	destination->WriteParameter(std::string("zoom_speed"), mZoomSpeed);
}