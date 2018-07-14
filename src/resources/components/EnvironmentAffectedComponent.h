#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>

class EnvironmentAffectedComponent : public IComponent
{
	bool mIsAffectedByWind;
	float mWindResistence;

public:
	EnvironmentAffectedComponent();
	~EnvironmentAffectedComponent();

	EnvironmentAffectedComponent* DoClone() const override;

	float GetWindResistence() const;
	void SetWindResistence(float resistence);
	void SetAffectedByWind(bool isAffected);
	bool IsAffectedByWind() const;
};

