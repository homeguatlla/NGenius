#include "stdafx.h"
#include "GameEntity.h"
#include "../renderer/IRenderer.h"


#include "components\PhysicsComponent.h"

GameEntity::GameEntity(Transformation* transformation, IRenderer* renderer) :
mTransformation(transformation),
mRenderer(renderer),
mIsEnabled(true)
{
	if (renderer != nullptr)
	{
		renderer->SetParent(this);
	}
}

GameEntity::GameEntity(Transformation* transformation) : GameEntity(transformation, nullptr)
{
}

GameEntity::~GameEntity()
{
	for (auto& pair : mComponents)
	{
		delete pair.second;
	}
	mComponents.clear();
	if (mTransformation != nullptr)
	{
		delete mTransformation;
		mTransformation = nullptr;
	}
	if (mRenderer != nullptr)
	{
		delete mRenderer;
		mRenderer = nullptr;
	}
}

Transformation* GameEntity::GetTransformation()
{
	return mTransformation;
}
const Transformation* GameEntity::GetTransformation() const
{
	return mTransformation;
}

IRenderer* GameEntity::GetRenderer() const
{
	return mRenderer;
}

void GameEntity::SetRenderer(IRenderer* renderer)
{
	mRenderer = renderer;
	if (renderer != nullptr)
	{
		renderer->SetParent(this);
	}
}

bool GameEntity::IsEnabled() const
{
	return mIsEnabled;
}

void GameEntity::SetEnabled(bool enabled)
{
	mIsEnabled = enabled;
}

GameEntity* GameEntity::DoClone() const
{
	GameEntity* clone = new GameEntity(new Transformation(*GetTransformation()));
	if (GetRenderer() != nullptr)
	{
		clone->SetRenderer(GetRenderer()->Clone());
	}
	
	return clone;
}

GameEntity* GameEntity::Clone()
{
	GameEntity* clone = DoClone();

	assert(typeid(*clone) == typeid(*this));

	for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{		
		clone->AddComponent(it->first, it->second->Clone());
	}

	return clone;
}

void GameEntity::Update(float elapsedTime)
{
	if (mIsEnabled)
	{
		for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			it->second->Update(elapsedTime);
		}
	}
}


