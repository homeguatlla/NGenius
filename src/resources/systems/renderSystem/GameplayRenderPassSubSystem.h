#pragma once

class RenderSystem;
class ICamera;
class RenderPass;
class IFrameBuffer;

class GameplayRenderPassSubSystem
{
	RenderSystem* mRenderSystem;
	ICamera* mCamera; 
	RenderPass* mRenderPass;
	IFrameBuffer* mFrameBuffer;
	bool mIsEnabled;
	bool mIsInitialized;

public:
	GameplayRenderPassSubSystem(RenderSystem* renderSystem, ICamera* camera);
	~GameplayRenderPassSubSystem();

	void Init();
	void SetEnable(bool enable);

private:
	RenderPass* CreateGUIRenderPass(ICamera* camera);
	IFrameBuffer* CreateFrameBuffer();
};

