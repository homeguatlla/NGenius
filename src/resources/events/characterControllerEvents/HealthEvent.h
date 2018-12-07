#pragma once
#include "../CharacterControllerEvent.h"

class InventoryItem;

class HealthEvent : public CharacterControllerEvent
{
	bool mIsDamage;
	float mHealth;
	float mMaxHealth;

public:
	HealthEvent(bool isDamage, float maxHealth, float health);
	~HealthEvent();

	HealthEvent* DoClone(const void* data) const override;

	bool IsDamage() const;
	float GetHealth() const;
	float GetMaxHealth() const;
	void SetDamage(bool damage);
	void SetHealth(float maxHealth, float health);
};

