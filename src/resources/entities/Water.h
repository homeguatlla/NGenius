#pragma once
#include "src/resources/BaseGameEntity.h"

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
	Water() = default;
	explicit Water(Transformation* transformation, IMaterial* material, float wide, float length, float speed);
	~Water();
	void Update(float elapsedTime) override;
	std::shared_ptr<IGameEntity> DoClone() const override { return nullptr; }

	void Build(NGenius* engine) override;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

	static std::string GetClassName() { return std::string("water"); }
	static std::shared_ptr<IGameEntity> DoCreate();

private:
	
	void CreateWater(IMaterial* material);
	void CreateModel();

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

