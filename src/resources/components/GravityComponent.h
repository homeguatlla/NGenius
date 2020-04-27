#pragma once
#include "IComponent.h"
#include "../../../../NPhysics/source/IForceGenerator.h"
#include "../../../../NPhysics/source/particle/Particle.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

class GravityComponent : public IComponent
{
public:
	GravityComponent(const glm::vec3& gravity);
	virtual ~GravityComponent() = default;
	
	glm::vec3 GetGravity() const { return mGravity; }
	static std::string GetClassName() { return std::string("GravityComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	GravityComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	GravityComponent* DoClone() const override;

private:
	glm::vec3 mGravity;
};

