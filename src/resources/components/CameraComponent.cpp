#include "stdafx.h"
#include "CameraComponent.h"
#include "../camera/ICamera.h"

CameraComponent::CameraComponent(ICamera* camera) : mCamera(camera)
{
}


CameraComponent::~CameraComponent()
{
}

CameraComponent* CameraComponent::DoClone() const
{
	return new CameraComponent(*this);
}
