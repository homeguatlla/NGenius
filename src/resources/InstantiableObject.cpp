#include "stdafx.h"
#include "InstantiableObject.h"

std::map<std::string, IFactory*> InstantiableObject::mFactories;

//TODO igual podríamos registrar una función statica de creación o algo así. Así, no tenemos que hacer un new Terrain por ejemplo y 
//podemos mantener el constructor por defecto privado.
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

ICamera* InstantiableObject::CreateCamera(const std::string& name)
{
	if (mFactories.find(name) != mFactories.end())
	{
		return mFactories[name]->CreateCamera();
	}
	else
	{
		return nullptr;
	}
}
