#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include<string>

class IGameEntity;

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

	static std::string GetClassName() { return std::string("EnergyWallCollisionComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	EnergyWallCollisionComponent* DoClone() const override;
};

