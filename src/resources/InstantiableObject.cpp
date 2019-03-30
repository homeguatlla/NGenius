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
}