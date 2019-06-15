#pragma once
#include "../GameEntity.h"

class IMaterial;
class Model;
class MaterialEffectWater;
class RenderSystem;

class Water :	public GameEntity
{
	float mWide;
	float mLength;
	float mWaterSpeed;
	float mCurrentWaterSpeed;
	Model* mModel;
	MaterialEffectWater* mWaterEffect;

public:
	Water() = default; 
	explicit Water(Transformation* transformation, IMaterial* material, float wide, float length, float speed);
	~Water();
	void Update(float elapsedTime) override;
	Water* DoClone() const override { return nullptr; }

	GameEntity* CreateGameEntity()  override;
	void Build(RenderSystem* renderSystem) override;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

private:
	
	void Create(IMaterial* material);
	void CreateModel();
};

