#include "stdafx.h"
#include "AnimationSystem.h"
#include "src/resources/IGameEntity.h"
#include "src/resources/components/AnimationComponent.h"
#include "src/resources/models/animation/AnimatedModel.h"
#include "src/resources/models/animation/Animation.h"
#include "src/resources/models/animation/Animator.h"
#include "src/resources/renderers/IRenderer.h"
#include "src/resources/materials/IMaterial.h"
#include "src/resources/materials/effects/MaterialEffectMatrix4Array.h"
#include "Memory.h"

#include <algorithm>

AnimationSystem::AnimationSystem()
{
}

AnimationSystem::~AnimationSystem()
{
	Release();
}

void AnimationSystem::Release()
{
	mEntities.clear();
}

void AnimationSystem::Reload()
{
	Release();
}

void AnimationSystem::Update(float deltaTime)
{
	for (std::pair<std::shared_ptr<IGameEntity>, Animator*> elem : mEntities)
	{
		std::shared_ptr<IGameEntity> entity = elem.first;
		Animator* animator = elem.second;

		AnimationComponent* animationComponent = entity->GetComponent<AnimationComponent>();
		assert(animationComponent != nullptr);

		//TODO hay que ver esto bien. Ahora suponemos que solo una animación puede estar activa
		Animation* animation = animationComponent->GetCurrentAnimation();
		animation != nullptr ? animator->BindAnimation(animation) : animator->ReleaseAnimation();
		animator->Update(deltaTime);
		SetAnimationData(entity, animator->GetJointTransforms());
	}
}

void AnimationSystem::SetAnimationData(std::shared_ptr<IGameEntity> entity, const std::vector<glm::mat4x4>& data)
{
	//Set animation parameters
	IRenderer* renderer = entity->GetRenderer();
	assert(renderer != nullptr);
	IMaterial* material = renderer->GetMaterial();
	MaterialEffectMatrix4Array* effectMatrix4Array = material->GetEffect<MaterialEffectMatrix4Array>();
	if (effectMatrix4Array != nullptr)
	{
		effectMatrix4Array->SetValues(data);
	}
}

void AnimationSystem::AddEntity(std::shared_ptr<IGameEntity> entity)
{
	if (HasAnimationComponents(entity))
	{
		AnimatedModel* model = static_cast<AnimatedModel*>(entity->GetRenderer()->GetModel());
		Animator* animator = DBG_NEW Animator(model);

		//AnimationComponent* component = entity->GetComponent<AnimationComponent>();
		mEntities.push_back(std::pair<std::shared_ptr<IGameEntity>, Animator*>(entity, animator));
		//mAnimations[model->GetName()].push_back(component->GetAnimation());
	}
}

void AnimationSystem::RemoveEntity(std::shared_ptr<IGameEntity> entity)
{
	if (HasAnimationComponents(entity))
	{
		std::vector<std::pair<std::shared_ptr<IGameEntity>, Animator*>>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](std::pair<std::shared_ptr<IGameEntity>, Animator*> a) { return a.first == entity; });
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

bool AnimationSystem::HasAnimationComponents(const std::shared_ptr<IGameEntity> entity) const
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

void AnimationSystem::OnGameEntityAdded(std::shared_ptr<IGameEntity> entity)
{
	if (HasAnimationComponents(entity))
	{
		AddEntity(entity);
	}
}

void AnimationSystem::OnGameEntityRemoved(std::shared_ptr<IGameEntity> entity)
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