#include "stdafx.h"
#include "HealthComponent.h"

#include <glm/glm.hpp>

HealthComponent::HealthComponent(float maxHealth) : mMaxHealth(maxHealth), mHealth(maxHealth)
{
}


HealthComponent::~HealthComponent()
{
}

float HealthComponent::GetHealth() const
{
	return mHealth;
}

float HealthComponent::GetMaxHealth() const
{
	return mMaxHealth;
}

void HealthComponent::SetHealth(float health)
{
	mHealth = health;
}

void HealthComponent::ApplyDamage(float damage)
{
	mHealth = glm::max(0.0f, mHealth - damage);
}

bool HealthComponent::IsDeath() const
{
	return mHealth <= 0.0f;
}

IComponent * HealthComponent::DoClone() const
{
	return new HealthComponent(*this);
}