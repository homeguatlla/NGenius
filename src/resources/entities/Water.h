#pragma once
#include "../GameEntity.h"

class IMaterial;
class Model;
class MaterialEffectWater;

class Water :	public GameEntity
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

private:
	void Create();
};

