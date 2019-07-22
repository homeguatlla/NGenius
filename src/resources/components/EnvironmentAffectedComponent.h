#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include<string>

class IGameEntity;

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

	static std::string GetClassName() { return std::string("EnvironmentAffectedComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	EnvironmentAffectedComponent* DoClone() const override;
};

