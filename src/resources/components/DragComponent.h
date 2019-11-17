#pragma once
#include "IComponent.h"

#include "../../../../NPhysics/source/IForceGenerator.h"
#include "../../../../NPhysics/source/particle/Particle.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

class DragComponent : public IComponent
{
public:
	DragComponent(float k1, float k2);
	virtual ~DragComponent() = default;

	float GetK1() const { return mK1; }
	float GetK2() const { return mK2; }

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

