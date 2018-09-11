#include "stdafx.h"
#include "AnimationComponent.h"
#include <assert.h>

AnimationComponent::AnimationComponent(Animation* animation, AnimatedModel* animatedModel) :
	mAnimation(animation),
	mAnimatedModel(animatedModel)
{
	assert(animation != nullptr);
	assert(animatedModel != nullptr);
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

AnimatedModel* AnimationComponent::GetAnimatedModel() const
{
	return mAnimatedModel;
}
