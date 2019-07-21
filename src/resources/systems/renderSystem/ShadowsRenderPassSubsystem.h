#pragma once
#include <glm/glm.hpp>
#include "../../../utils/serializer/ISerializable.h"

class ICamera;
class Texture;
class RenderPass;
class RenderSystem;
class IGameEntity;

class ShadowsRenderPassSubSystem : public core::utils::ISerializable
{
	RenderSystem* mRenderSystem;
	ICamera* mShadowCastCamera;
	Texture* mShadowMapTexture;
	RenderPass* mRenderPass;
	const IGameEntity* mTarget;
	glm::mat4 mShadowMapMatrix;
	glm::vec3 mDirectionalLightDirection;
	int mPFCCounter;
	float mScreenWidth;
	float mScreenHeight;
	bool mIsShadowCastEnabled;
	bool mIsInitialized;

public:
	ShadowsRenderPassSubSystem(RenderSystem* renderSystem, float screenWidth, float screenHeight);
	~ShadowsRenderPassSubSystem();

	void Init();
	void Update();

	void SetCastingShadowsTarget(const IGameEntity* target);
	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetEnable(bool enable);

	bool IsEnabled() const;
	const glm::mat4 GetShadowMapMatrix() const;
	const Texture* GetShadowMapTexture() const;
	int GetShadowMapPFCCounter() const;
	void UpdateState();

	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer* source) override;
	virtual void WriteTo(core::utils::ISerializer* destination) override;

private:
	ICamera* CreateShadowCastCamera(const glm::vec3& directionalLightDirection);
	glm::mat4 CalculateShadowMapMatrix(const ICamera* camera);
	RenderPass* CreateShadowRenderPass();
	void UpdateShadowCastMatrix();
	bool HasTarget() const { return mTarget != nullptr; }
};

