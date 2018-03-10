#pragma once
#include <glm/glm.hpp>

class ICamera;
class Texture;
class RenderPass;
class RenderSystem;

class WaterRenderPass
{
	ICamera* mReflectionCamera;
	ICamera* mRefractionCamera;
	const ICamera* mGameplayCamera;
	RenderSystem* mRenderSystem;
	RenderPass* mReflectionRenderPass;
	RenderPass* mRefractionRenderPass;
	float mScreenWidth;
	float mScreenHeight;
	bool mIsWaterEnabled;
	bool mIsInitialized;
	float mWaterY;

public:
	WaterRenderPass(RenderSystem* renderSystem, float screenWidth, float screenHeight);
	~WaterRenderPass();

	void Init();
	void Update();

	void SetWaterParameters(const ICamera* gameplayCamera, float waterY);
	void SetEnable(bool enable);
	bool IsEnabled() const;

private:
	ICamera* CreateReflectionCamera();
	ICamera* CreateRefractionCamera();
	RenderPass* CreateReflectionRenderPass();
	RenderPass* CreateRefractionRenderPass();
	void ApplyReflectionCameras(float yReflectionPlane, const ICamera* camera, ICamera* cameraReflected);
	void ApplyRefractionCameras(const ICamera* camera, ICamera* cameraRefracted);
};

