#include "stdafx.h"
#include "AnimationSystem.h"
#include "../GameEntity.h"
#include "../components/AnimationComponent.h"
#include "../models/animation/AnimatedModel.h"
#include "../models/animation/Animation.h"
#include "../models/animation/Animator.h"

#include <algorithm>

AnimationSystem::AnimationSystem()
{
}

AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Update(float deltaTime)
{
	for (std::pair<GameEntity*, Animator*> elem : mEntities)
	{
		elem.second->Update(deltaTime);
	}
}

void AnimationSystem::AddEntity(GameEntity* entity)
{
	if (HasAnimationComponents(entity))
	{
		AnimationComponent* component = entity->GetComponent<AnimationComponent>();
		Animator* animator = new Animator(component->GetAnimatedModel());

		mEntities.push_back(std::pair<GameEntity*, Animator*>(entity, animator));
		mAnimations[component->GetAnimatedModel()->GetName()].push_back(component->GetAnimation());

		//TODO por ahora lo hacemos aquí, pero esto se tendría que hacer a través del AnimatorComponent
		animator->PlayAnimation(component->GetAnimation());
	}
}

void AnimationSystem::RemoveEntity(GameEntity* entity)
{
	if (HasAnimationComponents(entity))
	{
		std::vector<std::pair<GameEntity*, Animator*>>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](std::pair<GameEntity*, Animator*> a) { return a.first == entity; });
		if (it != mEntities.end())
		{
			delete it->second;
			mEntities.erase(it);
		}
		else
		{
			assert(false);
		}
	}
}

bool AnimationSystem::HasAnimationComponents(const GameEntity* entity) const
{
	return entity != nullptr && (entity->HasComponent<AnimationComponent>());
}

void AnimationSystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasAnimationComponents(entity))
	{
		AddEntity(entity);
	}
}

void AnimationSystem::OnGameEntityRemoved(GameEntity* entity)
{
	if (HasAnimationComponents(entity))
	{
		RemoveEntity(entity);
	}
}

BaseVisitable<>::ReturnType AnimationSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}