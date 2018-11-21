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

	float GetWindResistence() const;
	void SetWindResistence(float resistence);
	void SetAffectedByWind(bool isAffected);
	bool IsAffectedByWind() const;

private:
	EnvironmentAffectedComponent* DoClone() const override;
};

