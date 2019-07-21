#include "stdafx.h"
#include "InstantiableObject.h"

std::map<std::string, IFactory*> InstantiableObject::mFactories;
std::map<std::string, InstantiableObject::RendererCreatorFunction> InstantiableObject::mRenderersFactory;
std::map<std::string, InstantiableObject::CameraCreatorFunction> InstantiableObject::mCamerasFactory;
std::map<std::string, InstantiableObject::ShaderCreatorFunction> InstantiableObject::mShadersFactory;
std::map<std::string, InstantiableObject::MaterialEffectCreatorFunction> InstantiableObject::mMaterialEffectFactory;
std::map<std::string, InstantiableObject::ComponentCreatorFunction> InstantiableObject::mComponentsFactory;
std::map<std::string, InstantiableObject::GameEntityCreatorFunction> InstantiableObject::mGameEntitiesFactory;

IMaterialEffect* InstantiableObject::CreateMaterialEffect(const std::string& name, IMaterial* material)
{
	if (mMaterialEffectFactory.find(name) != mMaterialEffectFactory.end())
	{
		return mMaterialEffectFactory[name](material);
	}
	else
	{
		return nullptr;
	}
}

IGameEntity* InstantiableObject::CreateEntity(const std::string& name)
{
	if (mGameEntitiesFactory.find(name) != mGameEntitiesFactory.end())
	{
		return mGameEntitiesFactory[name]();
	}
	else
	{
		return nullptr;
	}
}

ICamera* InstantiableObject::CreateOrthogonalCamera(const std::string& name, float screenWidth, float screenHeight, float nearPlane, float farPlane)
{
	if (mCamerasFactory.find(name) != mCamerasFactory.end())
	{
		return mCamerasFactory[name](name, screenWidth, screenHeight, nearPlane, farPlane);
	}
	else
	{
		return nullptr;
	}
}

ICamera* InstantiableObject::CreatePerspectiveCamera(const std::string& name, float fov, float aspectRatio, float nearPlane, float farPlane)
{
	if (mCamerasFactory.find(name) != mCamerasFactory.end())
	{
		return mCamerasFactory[name](name, fov, aspectRatio, nearPlane, farPlane);
	}
	else
	{
		return nullptr;
	}
}

IRenderer* InstantiableObject::CreateRenderer(const std::string& name, Model* model, IMaterial* material)
{
	if (mRenderersFactory.find(name) != mRenderersFactory.end())
	{
		return mRenderersFactory[name](model, material);
	}
	else
	{
		return nullptr;
	}
}

IShaderProgram* InstantiableObject::CreateShader(const std::string& name)
{
	if (mShadersFactory.find(name) != mShadersFactory.end())
	{
		return mShadersFactory[name]();
	}
	else
	{
		return nullptr;
	}
}

IComponent* InstantiableObject::CreateComponent(const std::string& name, IGameEntity* entity)
{
	if (mComponentsFactory.find(name) != mComponentsFactory.end())
	{
		return mComponentsFactory[name](entity);
	}
	else
	{
		return nullptr;
	}
}
