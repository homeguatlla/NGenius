#pragma once

class RenderSystem;
class ICamera;
class RenderPass;

class GUIRenderPassSubSystem
{
	RenderSystem* mRenderSystem;
	ICamera* mCamera; 
	RenderPass* mRenderPass;
	float mScreenWidth;
	float mScreenHeight;
	bool mIsEnabled;
	bool mIsInitialized;

public:
	GUIRenderPassSubSystem(RenderSystem* renderSystem, float screenWidth, float screenHeight);
	~GUIRenderPassSubSystem();

	void Init();
	void SetEnable(bool enable);

private:
	ICamera* CreateCamera();
	RenderPass* CreateGUIRenderPass(ICamera* camera);
};

