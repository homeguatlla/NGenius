#pragma once
#include "../GameEntity.h"

class Model;
class IMaterial;

class EnergyWall : public GameEntity
{
	IMaterial* mMaterial;
	Model* mModel;
	float mMaxLiveTime;
	float mLiveTime;
	glm::vec3 mContactPoint;

public:
	explicit EnergyWall(Transformation* transformation, IMaterial* material, Model* model, float maxLiveTime);
	~EnergyWall();

	EnergyWall* DoClone() const override;

	void Update(float elapsedTime) override;

	void SetLiveTime(float liveTime);
	void SetMaxLiveTime(float maxLiveTime);
	void SetContactPoint(const glm::vec3& contact);

	float GetLiveTime() const;
	float GetMaxLiveTime() const;
	glm::vec3 GetContactPoint() const;

	bool IsAlive() const;
};

