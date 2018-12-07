#include "stdafx.h"
#include "HealthEvent.h"

HealthEvent::HealthEvent(bool isDamage, float health, float maxHealth) : mIsDamage(isDamage), mHealth(health), mMaxHealth(maxHealth)
{
}


HealthEvent::~HealthEvent()
{
}

HealthEvent* HealthEvent::DoClone(const void* data) const
{
	HealthEvent* healthEvent = new HealthEvent(mIsDamage, mHealth, mMaxHealth);

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


void HealthEvent::SetDamage(bool damage)
{
	mIsDamage = damage;
}

void HealthEvent::SetHealth(float health)
{
	mHealth = health;
}

void HealthEvent::SetMaxHealth(float maxHealth)
{
	mMaxHealth = maxHealth;
}

float HealthEvent::GetMaxHealth() const
{
	return mMaxHealth;
}