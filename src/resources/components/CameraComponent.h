#pragma once
#include "IComponent.h"

class ICamera;

class CameraComponent : public IComponent
{
	ICamera* mCamera;
public:
	CameraComponent(ICamera* camera);
	~CameraComponent();

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	CameraComponent* DoClone() const override;
};

