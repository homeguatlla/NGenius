#pragma once
#include "IComponent.h"
#include<string>

class ICamera;

class CameraComponent : public IComponent
{
	ICamera* mCamera;
public:
	CameraComponent(ICamera* camera);
	~CameraComponent();

	static std::string GetClassName() { return std::string("CameraComponent"); }
	static IComponent* Create();

private:
	CameraComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	CameraComponent* DoClone() const override;
};

