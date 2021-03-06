#pragma once
#include "../../postprocesseffects/PostProcessEffect.h"
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
	void Render();

	void AddPostProcessEffect(PostProcessEffect* postProcessEffect);
	void RemovePostProcessEffect(PostProcessEffect* postProcessEffect);

private:
	void Load();
	void LoadContrastPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, unsigned int width, unsigned int height);
	void LoadBrightPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, const glm::vec3& bright, unsigned int width, unsigned int height);
	void LoadHorizontalBlurPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, float blurGranularity, unsigned int width, unsigned int height);
	void LoadVerticalBlurPostProcess(Model* model, Texture* texture, PostProcessEffect::PostProcessEffectType type, float blurGranularity, unsigned int width, unsigned int height);
	void LoadBloomPostProcess(Model* model, Texture* texture, Texture* bright, PostProcessEffect::PostProcessEffectType type, unsigned int width, unsigned int height);
	void ReleaseEffects();
};

