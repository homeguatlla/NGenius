#include "stdafx.h"
#include "ThirdPersonCameraComponent.h"

#include <glm/glm.hpp>
#include "../GameEntity.h"
#include "../camera/PerspectiveCamera.h"
#include "../GameEvent.h"
#include "CollisionComponent.h"
#include "OverWaterComponent.h"
#include "PlayerInputComponent.h"

//WARNING!! hay que tener en cuenta que, si subimos este valor la cámara por colisión subirá y no mantendrá el ángulo pitch que le hemos definido.
//es decir, si vemos que el ángulo de la cámara con el target es demasiado alto (vemos al player desde una posición más alta) hay que tener en cuenta que haya subido por este valor
const float CAMERA_HEIGHT_OFFSET_GROUND = 0.2f;
const float CAMERA_SMOOTH_MOVEMENT_VALUE = 1.0f;
const float WATER_HEIGHT_OFFSET = 0.1f;

ThirdPersonCameraComponent::ThirdPersonCameraComponent(PerspectiveCamera* camera, GameEntity* target, float distanceFromTarget, float pitch, float zoomSpeed) :
	mCamera(camera), mTarget(target), mDistanceFromTarget(distanceFromTarget), mPitch(pitch), mAngleAroundTarget(0.0f), mZoomSpeed(zoomSpeed)
{
	assert(target != nullptr);
}

ThirdPersonCameraComponent::~ThirdPersonCameraComponent()
{
}

ThirdPersonCameraComponent* ThirdPersonCameraComponent::DoClone() const
{
	return new ThirdPersonCameraComponent(*this);
}

void ThirdPersonCameraComponent::Update(float elapsedTime)
{
	glm::vec3 newTarget = mTarget->GetTransformation()->GetPosition();
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

	mCamera->SetPosition(newPosition);
	mParent->GetTransformation()->SetPosition(newPosition);
}

const GameEntity* ThirdPersonCameraComponent::GetTarget() const
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

float ThirdPersonCameraComponent::CalculateHorizontalDistance() const
{
	return mDistanceFromTarget * glm::cos(glm::radians(mPitch));
}

float ThirdPersonCameraComponent::CalculateVerticalDistance() const
{
	return mDistanceFromTarget * glm::sin(glm::radians(mPitch));
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

void ThirdPersonCameraComponent::UpdateZoomSpeed(float scroll)
{
	mCurrentZoomSpeed = mZoomSpeed * scroll;
	mDistanceFromTarget += mCurrentZoomSpeed;
}