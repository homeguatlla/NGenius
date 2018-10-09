#include "stdafx.h"
#include "AnimationComponent.h"
#include "../models/animation/Animation.h"
#include <assert.h>
#include <algorithm>

AnimationComponent::AnimationComponent() :
	mCurrentAnimation(nullptr)
{
}

AnimationComponent::~AnimationComponent()
{
}

AnimationComponent* AnimationComponent::DoClone() const
{
	return new AnimationComponent(*this);
}

void AnimationComponent::AddAnimation(Animation* animation)
{
	assert(animation != nullptr);

	bool found = mAnimations.find(animation->GetName()) != mAnimations.end();
	if (!found)
	{
		mAnimations[animation->GetName()] = animation;
	}
}

Animation* AnimationComponent::GetCurrentAnimation()
{
	return mCurrentAnimation;
}

void AnimationComponent::PlayAnimation(const std::string& name)
{
	if (mCurrentAnimation == nullptr || mCurrentAnimation->GetName() != name)
	{
		std::map<std::string, Animation*>::iterator it = mAnimations.find(name);
		if (it != mAnimations.end())
		{
			mCurrentAnimation = it->second;
		}
	}
}

void AnimationComponent::StopAnimation()
{
	mCurrentAnimation = nullptr;
}