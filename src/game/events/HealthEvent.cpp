#include "stdafx.h"
#include "HealthEvent.h"
#include "../../resources/systems/InputSystem.h"

HealthEvent::HealthEvent(bool isDamage, float maxHealth, float health) : mIsDamage(isDamage), mHealth(health), mMaxHealth(maxHealth)
{
}


HealthEvent::~HealthEvent()
{
}

HealthEvent* HealthEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	HealthEvent* healthEvent = new HealthEvent(mIsDamage, mMaxHealth, mHealth);

	return healthEvent;
}


bool HealthEvent::IsDamage() const
{
	return mIsDamage;
}

float HealthEvent::GetHealth() const
{
	return mHealth;
}

float HealthEvent::GetMaxHealth() const
{
	return mMaxHealth;
}

void HealthEvent::SetDamage(bool damage)
{
	mIsDamage = damage;
}

void HealthEvent::SetHealth(float maxHealth, float health)
{
	mMaxHealth = maxHealth;
	mHealth = health;
}