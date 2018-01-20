#pragma once
#include "../GameEntity.h"

class EnergyWall : public GameEntity
{
	float mMaxLiveTime;
	float mLiveTime;
	glm::vec3 mContactPoint;

public:
	explicit EnergyWall(Transformation* transformation, IRenderer* renderer, float maxLiveTime);
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

