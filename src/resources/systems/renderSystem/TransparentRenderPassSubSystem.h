#pragma once

class RenderSystem;
class ICamera;
class RenderPass;

class TransparentRenderPassSubSystem
{
	RenderSystem* mRenderSystem;
	ICamera* mCamera; 
	RenderPass* mRenderPass;
	bool mIsEnabled;
	bool mIsInitialized;

public:
	TransparentRenderPassSubSystem(RenderSystem* renderSystem, ICamera* camera);
	~TransparentRenderPassSubSystem();

	void Init();
	void SetEnable(bool enable);

private:
	RenderPass* CreateGUIRenderPass(ICamera* camera);
};

