#pragma once
#include "IComponent.h"
#include<string>

class IGameEntity;

class OverWaterComponent : public IComponent
{
	float mWaterHeight;

public:
	OverWaterComponent(float waterHeight);
	~OverWaterComponent();

	void Init(GameScene* scene, RenderSystem* renderSystem);

	float GetWaterHeight() const;

	static std::string GetClassName() { return std::string("OverWaterComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	OverWaterComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	OverWaterComponent* DoClone() const;
};

