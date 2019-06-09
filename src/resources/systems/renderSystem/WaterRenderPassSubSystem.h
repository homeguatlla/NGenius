#pragma once
#include <glm/glm.hpp>
#include "../../../utils/serializer/ISerializable.h"
#include <string>

class ICamera;
class Texture;
class RenderPass;
class RenderSystem;

class WaterRenderPassSubSystem : public core::utils::ISerializable
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

	std::string mCameraName;

public:
	WaterRenderPassSubSystem(RenderSystem* renderSystem, float screenWidth, float screenHeight);
	~WaterRenderPassSubSystem();

	void Init();
	void Update();

	void SetWaterParameters(const ICamera* gameplayCamera, float waterY);
	void SetEnable(bool enable);
	bool IsEnabled() const;

	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer* source) override;
	virtual void WriteTo(core::utils::ISerializer* destination) override;

private:
	ICamera* CreateReflectionCamera();
	ICamera* CreateRefractionCamera();
	RenderPass* CreateReflectionRenderPass();
	RenderPass* CreateRefractionRenderPass();
	void ApplyReflectionCameras(float yReflectionPlane, const ICamera* camera, ICamera* cameraReflected);
	void ApplyRefractionCameras(const ICamera* camera, ICamera* cameraRefracted);
};

