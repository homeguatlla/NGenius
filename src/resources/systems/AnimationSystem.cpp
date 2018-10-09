#include "stdafx.h"
#include "AnimationSystem.h"
#include "../GameEntity.h"
#include "../components/AnimationComponent.h"
#include "../models/animation/AnimatedModel.h"
#include "../models/animation/Animation.h"
#include "../models/animation/Animator.h"
#include "../renderers/IRenderer.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectMatrix4Array.h"

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
		GameEntity* entity = elem.first;
		Animator* animator = elem.second;

		AnimationComponent* animationComponent = entity->GetComponent<AnimationComponent>();
		assert(animationComponent != nullptr);

		//TODO hay que ver esto bien. Ahora suponemos que solo una animación puede estar activa
		Animation* animation = animationComponent->GetCurrentAnimation();
		if (animation != nullptr)
		{
			animator->BindAnimation(animation);
			animator->Update(deltaTime);

			//Set animation parameters
			IRenderer* renderer = entity->GetRenderer();
			assert(renderer != nullptr);
			IMaterial* material = renderer->GetMaterial();
			MaterialEffectMatrix4Array* effectMatrix4Array = material->GetEffect<MaterialEffectMatrix4Array>();
			if (effectMatrix4Array != nullptr)
			{
				effectMatrix4Array->SetValues(animator->GetJointTransforms());
			}
		}
		else
		{
			animator->ReleaseAnimation();
		}
	}
}

void AnimationSystem::AddEntity(GameEntity* entity)
{
	if (HasAnimationComponents(entity))
	{
		AnimatedModel* model = static_cast<AnimatedModel*>(entity->GetRenderer()->GetModel());
		Animator* animator = new Animator(model);

		AnimationComponent* component = entity->GetComponent<AnimationComponent>();
		mEntities.push_back(std::pair<GameEntity*, Animator*>(entity, animator));
		//mAnimations[model->GetName()].push_back(component->GetAnimation());
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
	if (entity != nullptr)
	{
		IRenderer* renderer = entity->GetRenderer();
		bool hasRenderer = renderer != nullptr;
		bool hasAnimationComponent = entity->HasComponent<AnimationComponent>();

		return hasRenderer && renderer->GetModel()->IsAnimatedModel() && hasAnimationComponent;
	}

	return false;
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