#pragma once
#include "../GameEntity.h"

class Particle : public GameEntity
{
	const float mMaxLiveTime;
	float mLiveTime;
	glm::vec2 mScale;
	float mRotationSpeed;
	float mRotation;
	float mDistanceToTheCamera;

	glm::vec4 mColor;
	glm::vec4 mColorOrigin;
	glm::vec4 mColorDestination;

public:
	Particle(Transformation* transformation, IRenderer* renderer, float liveTime);
	~Particle();

	Particle* DoClone() const override;

	bool IsAlive() const;
	void Update(float elapsedTime) override;
	void SetScaleFactor(glm::vec2& scale);
	void SetRotationSpeed(float rotationSpeed);
	void SetLiveTime(float liveTime);
	void SetColorGradientValues(glm::vec4 origin, glm::vec4 destination);
	const glm::vec4& GetColor() const;
	float GetMaxLiveTime() const;
	float GetLiveTime() const;

private:
	float Lerp(float p0, float p1, float t);
	glm::vec4 Lerp(glm::vec4& p0, glm::vec4& p1, float t);
};

