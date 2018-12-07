#pragma once
#include "../CharacterControllerEvent.h"

class InventoryItem;

class HealthEvent : public CharacterControllerEvent
{
	bool mIsDamage;
	float mHealth;
	float mMaxHealth;

public:
	HealthEvent(bool isDamage, float health, float MaxHealth);
	~HealthEvent();

	HealthEvent* DoClone(const void* data) const override;

	bool IsDamage() const;
	float GetHealth() const;
	void SetDamage(bool damage);
	void SetHealth(float health);
	void SetMaxHealth(float maxHealth);
	float GetMaxHealth() const;
};

