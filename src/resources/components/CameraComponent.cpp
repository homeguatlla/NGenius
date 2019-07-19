#include "stdafx.h"
#include "CameraComponent.h"
#include "../camera/ICamera.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../Memory.h"

CameraComponent::CameraComponent(ICamera* camera) : mCamera(camera)
{
}


CameraComponent::~CameraComponent()
{
}

IComponent* CameraComponent::Create()
{
	return DBG_NEW CameraComponent();
}

CameraComponent* CameraComponent::DoClone() const
{
	return DBG_NEW CameraComponent(*this);
}

void CameraComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void CameraComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("camera_component"));
	destination->WriteParameter(std::string("camera_name"), mCamera->GetName());
}