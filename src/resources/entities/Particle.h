#pragma once
#include "../BaseGameEntity.h"

class Model;
class IMaterial;

class Particle : public BaseGameEntity<Particle>
{
	const float mMaxLiveTime;
	float mLiveTime;
	glm::vec2 mScale;
	float mRotationSpeed;
	float mRotation;
	float mDistanceToTheCamera;

	Model* mModel;
	IMaterial* mMaterial;

	glm::vec4 mColor;
	glm::vec4 mColorOrigin;
	glm::vec4 mColorDestination;

public:
	Particle(Transformation* transformation, Model* model, IMaterial* material, float liveTime);
	~Particle();

	Particle* DoClone() const override;

	bool IsAlive() const;
	void Update(float elapsedTime) override;
	void SetScaleFactor(glm::vec2& scale);
	void SetRotationSpeed(float rotationSpeed);
	void SetLiveTime(float liveTime);
	void SetColorGradientValues(const glm::vec4& origin, const glm::vec4& destination);
	const glm::vec4& GetColor() const;
	float GetMaxLiveTime() const;
	float GetLiveTime() const;
};

