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
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	EnvironmentAffectedComponent* DoClone() const override;
};

