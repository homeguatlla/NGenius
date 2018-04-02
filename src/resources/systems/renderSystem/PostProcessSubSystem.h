#pragma once

#include <vector>

class RenderSystem;
class PostProcessEffect;

class PostProcessSubSystem
{
	typedef std::vector<PostProcessEffect*>::iterator PostProcessEffectIterator;
	std::vector<PostProcessEffect*> mEffects;
	RenderSystem* mRenderSystem;

public:
	PostProcessSubSystem(RenderSystem* renderSystem);
	~PostProcessSubSystem();

	void Init();
	void Render();

	void AddPostProcessEffect(PostProcessEffect* postProcessEffect);
	void RemovePostProcessEffect(PostProcessEffect* postProcessEffect);
};

