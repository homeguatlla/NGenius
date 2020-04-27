#pragma once
#include "../BaseGameEntity.h"

class Model;
class IMaterial;

class Particle : public BaseGameEntity<Particle>
{
	float mMaxLiveTime;
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
	Particle() = default;
	Particle(Transformation* transformation, Model* model, IMaterial* material, float liveTime);
	~Particle() = default;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	static std::string GetClassName() { return std::string("particle"); }
	static std::shared_ptr<IGameEntity> DoCreate();

	std::shared_ptr<IGameEntity> DoClone() const override;

	void Build(NGenius* engine) override;

	bool IsAlive() const;
	void Update(float elapsedTime) override;
	void SetScaleFactor(glm::vec2& scale);
	void SetRotationSpeed(float rotationSpeed);
	void SetLiveTime(float liveTime);
	void SetColorGradientValues(const glm::vec4& origin, const glm::vec4& destination);
	const glm::vec4& GetColor() const;
	float GetMaxLiveTime() const;
	float GetLiveTime() const;

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;

private:
	
	void CreateRenderer(Model* model, IMaterial* material);
};

