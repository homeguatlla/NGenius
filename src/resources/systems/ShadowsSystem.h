#pragma once
#include <glm/glm.hpp>

class ICamera;
class Texture;
class RenderPass;
class RenderSystem;

class ShadowsSystem
{
	RenderSystem* mRenderSystem;
	ICamera* mShadowCastCamera;
	const Texture* mShadowMapTexture;
	glm::mat4 mShadowMapMatrix;
	glm::vec3 mDirectionalLightDirection;
	int mPFCCounter;
	float mScreenWidth;
	float mScreenHeight;
	bool mIsShadowCastEnabled;

public:
	ShadowsSystem(RenderSystem* renderSystem, float screenWidth, float screenHeight, const Texture* shadowTexture);
	~ShadowsSystem();

	void Init();
	void SetCastingShadowsTarget(const glm::vec3& position);
	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);

	const glm::mat4 GetShadowMapMatrix() const;
	const Texture* GetShadowMapTexture() const;
	int GetShadowMapPFCCounter() const;

private:
	ICamera* CreateShadowCastCamera(const glm::vec3& directionalLightDirection);
	glm::mat4 CalculateShadowMapMatrix(const ICamera* camera);
	RenderPass* CreateShadowRenderPass();
	void UpdateShadowCastMatrix();
};

