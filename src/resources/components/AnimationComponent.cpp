#include "stdafx.h"
#include "AnimationComponent.h"
#include <assert.h>

AnimationComponent::AnimationComponent(Animation* animation) :
	mAnimation(animation)
{
	assert(animation != nullptr);
}

AnimationComponent::~AnimationComponent()
{
}

AnimationComponent* AnimationComponent::DoClone() const
{
	return new AnimationComponent(*this);
}

Animation* AnimationComponent::GetAnimation() const
{
	return mAnimation;
}