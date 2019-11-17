#pragma once
#include "IComponent.h"
#include "../../../../NPhysics/source/IForceGenerator.h"
#include "../../../../NPhysics/source/particle/Particle.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

class BuoyancyComponent : public IComponent
{
public:
	BuoyancyComponent(float liquidDensity);
	virtual ~BuoyancyComponent() = default;

	void Init(GameScene* scene, RenderSystem* renderSystem);
	
	float GetVolume() const { return mVolume; }
	float GetMaxDepth() const { return mMaxDepth; }
	float GetWaterHeight() const { return mWaterHeight; }
	float GetLiquidDensity() const { return mLiquidDensity; }
	glm::vec3 GetCenter() const { return mCenter; }

	static std::string GetClassName() { return std::string("BuoyancyComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	BuoyancyComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	BuoyancyComponent* DoClone() const override;

private:
	float mLiquidDensity;
	float mVolume;
	float mMaxDepth;
	float mWaterHeight;
	glm::vec3 mCenter;
};

