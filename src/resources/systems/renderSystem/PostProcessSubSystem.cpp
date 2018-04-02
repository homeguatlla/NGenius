#include "stdafx.h"
#include "PostProcessSubSystem.h"
#include "../../postprocesseffects/PostProcessEffect.h"

PostProcessSubSystem::PostProcessSubSystem(RenderSystem* renderSystem) :
	mRenderSystem(renderSystem)
{
}


PostProcessSubSystem::~PostProcessSubSystem()
{
}

void PostProcessSubSystem::Init()
{

}

void PostProcessSubSystem::Render()
{

}

void PostProcessSubSystem::AddPostProcessEffect(PostProcessEffect* postProcessEffect)
{
	bool found = std::find(mEffects.begin(), mEffects.end(), postProcessEffect) != mEffects.end();
	if (!found)
	{
		mEffects.push_back(postProcessEffect);
	}
}

void PostProcessSubSystem::RemovePostProcessEffect(PostProcessEffect* postProcessEffect)
{
	PostProcessEffectIterator it = std::find(mEffects.begin(), mEffects.end(), postProcessEffect);
	bool found = it != mEffects.end();
	if (found)
	{
		mEffects.erase(it);
	}
}
