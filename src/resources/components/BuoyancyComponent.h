#pragma once
#include "IComponent.h"

#include <glm/glm.hpp>

class GravityComponent : public IComponent
{
public:
	GravityComponent(const glm::vec3& gravity);
	virtual ~GravityComponent() = default;

private:
	glm::vec3 mGravity;
};

