#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>

class EnergyWallCollisionComponent : public IComponent
{
	bool mIsColliding;
	glm::vec3 mCollisionPoint;

public:
	explicit EnergyWallCollisionComponent();
	~EnergyWallCollisionComponent();

	bool IsColliding() const;
	void SetCollision(bool isColliding);
	void SetCollisionPoint(glm::vec3& position);
	glm::vec3 GetCollisionPoint() const;

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	EnergyWallCollisionComponent* DoClone() const override;
};

