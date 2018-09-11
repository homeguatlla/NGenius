#pragma once
#include "IComponent.h"

class Animation;
class AnimatedModel;

class AnimationComponent : public IComponent
{
	Animation* mAnimation;
	AnimatedModel* mAnimatedModel;
public:
	AnimationComponent(Animation* animation, AnimatedModel* animatedModel);
	~AnimationComponent();

	Animation* GetAnimation() const;
	AnimatedModel* GetAnimatedModel() const;

	AnimationComponent* DoClone() const override;
};

