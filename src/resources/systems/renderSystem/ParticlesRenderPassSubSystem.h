#pragma once

class RenderSystem;
class ICamera;
class RenderPass;

class ParticlesRenderPassSubSystem
{
	RenderSystem* mRenderSystem;
	ICamera* mCamera; 
	RenderPass* mRenderPass;
	float mScreenWidth;
	float mScreenHeight;
	bool mIsEnabled;
	bool mIsInitialized;

public:
	ParticlesRenderPassSubSystem(RenderSystem* renderSystem, ICamera* camera);
	~ParticlesRenderPassSubSystem();

	void Init();
	void SetEnable(bool enable);

private:
	RenderPass* CreateRenderPass(ICamera* camera);
};

