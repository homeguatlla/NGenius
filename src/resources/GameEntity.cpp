#include "stdafx.h"
#include "GameEntity.h"
#include "renderers/IRenderer.h"

#include "materials/IMaterial.h"
#include "models/Model.h"
#include "../utils/serializer/XMLSerializer.h"

unsigned GameEntity::IDCounter = 0;

GameEntity::GameEntity(Transformation* transformation, IRenderer* renderer) :
mTransformation(transformation),
mRenderer(renderer),
mIsEnabled(true)
{
	if (renderer != nullptr)
	{
		renderer->SetParent(this);
	}
	mID = ++IDCounter;
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

void GameEntity::Init()
{
	for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		it->second->Init();
	}
}

int GameEntity::GetID()
{
	return mID;
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

void GameEntity::ReadFrom(core::utils::IDeserializer* source)
{
}

void GameEntity::WriteTo(core::utils::ISerializer* destination)
{
	destination->BeginAttribute(std::string("entity"));
	destination->WriteParameter(std::string("id"), mID, "");
	destination->WriteParameter(std::string("is_enabled"), mIsEnabled);
	if (GetRenderer() != nullptr)
	{
		IRenderer* renderer = GetRenderer();
		unsigned int modelID = renderer->GetModel()->GetID();
		unsigned int materialID = renderer->GetMaterial()->GetMaterialID();
		destination->WriteParameter(std::string("modelID"), modelID);
		destination->WriteParameter(std::string("materialID"), materialID);
	}
	mTransformation->WriteTo(destination);

	destination->BeginAttribute(std::string("components"));
	for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		it->second->WriteTo(destination);
	}
	destination->EndAttribute();
	destination->EndAttribute();
}


