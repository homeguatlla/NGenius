#pragma once
#include "IComponent.h"

#include "../../../../NPhysics/source/particle/IParticleForceGenerator.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

class DragComponent : public IComponent
{
	std::shared_ptr<NPhysics::IParticleForceGenerator> mGenerator;

public:
	DragComponent(float k1, float k2);
	virtual ~DragComponent() = default;

	void Init(GameScene* scene, RenderSystem* renderSystem);
	std::shared_ptr<NPhysics::IParticleForceGenerator> GetGenerator() const { return mGenerator; }

	static std::string GetClassName() { return std::string("DragComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	DragComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	DragComponent* DoClone() const override;

private:
	float mK1;
	float mK2;
};

