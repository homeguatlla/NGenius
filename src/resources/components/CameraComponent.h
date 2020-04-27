#pragma once
#include "IComponent.h"
#include<string>

class ICamera;
class IGameEntity;

class CameraComponent : public IComponent
{
	ICamera* mCamera;
public:
	CameraComponent(ICamera* camera);
	~CameraComponent();

	static std::string GetClassName() { return std::string("CameraComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	CameraComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	CameraComponent* DoClone() const override;
};

