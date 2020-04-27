#pragma once
#include "IComponent.h"
#include<string>

class IGameEntity;

class EnvironmentModificatorComponent : public IComponent
{
public:
	EnvironmentModificatorComponent();
	virtual ~EnvironmentModificatorComponent();

	static std::string GetClassName() { return std::string("EnvironmentModificatorComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	EnvironmentModificatorComponent* DoClone() const;
};

