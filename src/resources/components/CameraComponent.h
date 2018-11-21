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
	CameraComponent* DoClone() const override;
};

