#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include<string>

class IGameEntity;

class RotationComponent : public IComponent
{
	glm::vec3 mRotation;
	float mSpeed;
	float mCurrentAngle;

public:
	explicit RotationComponent(const glm::vec3& rotation, float speed);
	~RotationComponent();

	void UpdateInternal(float elapsedTime) override;

	static std::string GetClassName() { return std::string("RotationComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	RotationComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	RotationComponent* DoClone() const override;
};

