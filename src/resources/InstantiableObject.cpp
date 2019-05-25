#include "stdafx.h"
#include "InstantiableObject.h"

std::map<std::string, IFactory*> InstantiableObject::mFactories;

void InstantiableObject::RegisterType(const std::string& name, IFactory* factory)
{
	mFactories[name] = factory;
}

IShaderProgram* InstantiableObject::CreateShader(const std::string& name)
{
	if (mFactories.find(name) != mFactories.end())
	{
		return mFactories[name]->CreateShader();
	}
	else
	{
		return nullptr;
	}
}

IMaterialEffect* InstantiableObject::AddNewEffectToMaterial(const std::string& name, IMaterial* material)
{
	if (mFactories.find(name) != mFactories.end())
	{
		return mFactories[name]->AddNewEffectToMaterial(material);
	}
	else
	{
		return nullptr;
	}
}

GameEntity* InstantiableObject::CreateEntity(const std::string& name)
{
	if (mFactories.find(name) != mFactories.end())
	{
		return mFactories[name]->CreateGameEntity();
	}
	else
	{
		return nullptr;
	}
}
