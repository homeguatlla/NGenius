#include "stdafx.h"
#include "AnimationComponent.h"
#include "../IGameEntity.h"
#include "../models/animation/Animation.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../Memory.h"

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
	return DBG_NEW AnimationComponent(*this);
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

IComponent* AnimationComponent::Create(IGameEntity* entity)
{
	AnimationComponent* component = DBG_NEW AnimationComponent();
	entity->AddComponent(component);
	return component;
}

void AnimationComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void AnimationComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("animation_component"));
}