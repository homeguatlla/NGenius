#pragma once
#include "IComponent.h"

class Animation;

class AnimationComponent : public IComponent
{
	Animation* mAnimation;
	
public:
	AnimationComponent(Animation* animation);
	~AnimationComponent();

	Animation* GetAnimation() const;

	AnimationComponent* DoClone() const override;
};

