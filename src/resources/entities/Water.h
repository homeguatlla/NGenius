#pragma once
#include "../BaseGameEntity.h"

class IMaterial;
class Model;
class MaterialEffectWater;
class RenderSystem;

class Water :	public BaseGameEntity<Water>
{
	float mWide;
	float mLength;
	float mWaterSpeed;
	float mCurrentWaterSpeed;
	Model* mModel;
	MaterialEffectWater* mWaterEffect;

public:
	explicit Water(Transformation* transformation, IMaterial* material, float wide, float length, float speed);
	~Water();
	void Update(float elapsedTime) override;
	Water* DoClone() const override { return nullptr; }

	void Build(NGenius* engine) override;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

	static std::string GetClassName() { return std::string("water"); }
	static IGameEntity* DoCreate();

private:
	Water() = default;
	void CreateWater(IMaterial* material);
	void CreateModel();

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

