#include "stdafx.h"
#include "GameEntity.h"
#include "renderers/IRenderer.h"

#include "materials/IMaterial.h"
#include "models/Model.h"
#include "systems/renderSystem/RenderSystem.h"
#include "renderers/VerticesRenderer.h"
#include "../utils/serializer/XMLSerializer.h"
#include "../utils/serializer/IDeserializer.h"
#include "InstantiableObject.h"
#include "../Memory.h"

/*

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


void GameEntity::SetEnabled(bool enabled)
{
	mIsEnabled = enabled;
}



*/

