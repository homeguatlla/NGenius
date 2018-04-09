#pragma once

#include <vector>

class RenderSystem;
class PostProcessEffect;
class IFrameBuffer;
class ITexture;

class PostProcessSubSystem
{
	typedef std::vector<PostProcessEffect*>::iterator PostProcessEffectIterator;
	std::vector<PostProcessEffect*> mEffects;
	RenderSystem* mRenderSystem;
	IFrameBuffer* mFrameBuffer;

public:
	PostProcessSubSystem(RenderSystem* renderSystem);
	~PostProcessSubSystem();

	void Init();
	void Render(ITexture* texture);

	void AddPostProcessEffect(PostProcessEffect* postProcessEffect);
	void RemovePostProcessEffect(PostProcessEffect* postProcessEffect);
};

