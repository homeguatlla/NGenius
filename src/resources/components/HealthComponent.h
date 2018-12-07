#pragma once
#include "../../resources/components/IComponent.h"

class HealthComponent :	public IComponent
{
	float mHealth;
	float mMaxHealth;

public:
	
	HealthComponent(float maxHealth);
	virtual ~HealthComponent();

	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float life);
	void ApplyDamage(float damage);
	bool IsDeath() const;

private:
	// Heredado vía IComponent
	IComponent * DoClone() const override;
};

