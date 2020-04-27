#pragma once
#include "IComponent.h"

#include <string>
#include <map>

class Animation;
class IGameEntity;

class AnimationComponent : public IComponent
{
	std::map<std::string, Animation*> mAnimations;
	Animation* mCurrentAnimation;
	std::string mAnimationName;

public:
	AnimationComponent();
	~AnimationComponent();

	void Init(GameScene* scene, RenderSystem* renderSystem) override;

	void AddAnimation(Animation* animation);
	Animation* GetCurrentAnimation();
	void PlayAnimation(const std::string& name);
	void StopAnimation();

	static std::string GetClassName() { return std::string("AnimationComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	AnimationComponent* DoClone() const override;
};

