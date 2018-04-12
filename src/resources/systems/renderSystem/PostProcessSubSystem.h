#pragma once

#include <vector>

class RenderSystem;
class PostProcessEffect;
class IFrameBuffer;
class ITexture;
class Model;
class Texture;

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

private:
	void Load();
	void LoadContrastPostProcess(Model* model, Texture* texture);
	void LoadHorizontalBlurPostProcess(Model* model, Texture* texture, float width);
	void LoadVerticalBlurPostProcess(Model* model, Texture* texture, float height);
};

