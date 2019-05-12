#pragma once
#include <glm/glm.hpp>

class ICamera;
class Texture;
class RenderPass;
class RenderSystem;
class GameEntity;

class ShadowsRenderPassSubSystem
{
	RenderSystem* mRenderSystem;
	ICamera* mShadowCastCamera;
	Texture* mShadowMapTexture;
	RenderPass* mRenderPass;
	const GameEntity* mTarget;
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

	void SetCastingShadowsTarget(const GameEntity* target);
	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetEnable(bool enable);

	bool IsEnabled() const;
	const glm::mat4 GetShadowMapMatrix() const;
	const Texture* GetShadowMapTexture() const;
	int GetShadowMapPFCCounter() const;
	void UpdateState();

private:
	ICamera* CreateShadowCastCamera(const glm::vec3& directionalLightDirection);
	glm::mat4 CalculateShadowMapMatrix(const ICamera* camera);
	RenderPass* CreateShadowRenderPass();
	void UpdateShadowCastMatrix();
	bool HasTarget() const { return mTarget != nullptr; }
};

