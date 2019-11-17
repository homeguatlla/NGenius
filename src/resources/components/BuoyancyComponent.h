#pragma once
#include "IComponent.h"
#include "../../../../NPhysics/source/IForceGenerator.h"
#include "../../../../NPhysics/source/particle/Particle.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

class BuoyancyComponent : public IComponent
{
	std::shared_ptr<NPhysics::IForceGenerator<NPhysics::Particle>> mGenerator;

public:
	BuoyancyComponent(float liquidDensity);
	virtual ~BuoyancyComponent() = default;

	void Init(GameScene* scene, RenderSystem* renderSystem);
	std::shared_ptr<NPhysics::IForceGenerator<NPhysics::Particle>> GetGenerator() const { return mGenerator; }

	static std::string GetClassName() { return std::string("BuoyancyComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	BuoyancyComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	BuoyancyComponent* DoClone() const override;

private:
	float mLiquidDensity;
};

