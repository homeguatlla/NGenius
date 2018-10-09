#pragma once
#include "IComponent.h"

#include <string>
#include <map>

class Animation;

class AnimationComponent : public IComponent
{
	std::map<std::string, Animation*> mAnimations;
	Animation* mCurrentAnimation;

public:
	AnimationComponent();
	~AnimationComponent();

	void AddAnimation(Animation* animation);
	Animation* GetCurrentAnimation();
	void PlayAnimation(const std::string& name);
	void StopAnimation();

	AnimationComponent* DoClone() const override;
};

