#pragma once
#include "IFactory.h"
#include <string>
#include <map>
#include <functional>
#include <memory>

class IShaderProgram;
class IMaterialEffect;
class IMaterial;
class Model;
class IRenderer;
class Transformation;
class IGameEntity;
class IComponent;
class GameEvent;

class InstantiableObject
{
private:
	using RendererCreatorFunction = std::function<IRenderer* (Model*, IMaterial*)> ;
	using CameraCreatorFunction = std::function<ICamera* (const std::string& name, float, float, float, float)>;
	using ShaderCreatorFunction = std::function<IShaderProgram* ()>;
	using MaterialEffectCreatorFunction = std::function<IMaterialEffect* (IMaterial*)>;
	using ComponentCreatorFunction = std::function<IComponent* (std::shared_ptr<IGameEntity>)>;
	using GameEntityCreatorFunction = std::function<std::shared_ptr<IGameEntity>()>;
	using GameEventCreatorFunction = std::function<std::shared_ptr<GameEvent> ()>;

	public:
		template<class T>
		static void RegisterMaterialEffectType()
		{
			mMaterialEffectFactory[T::GetClassName()] = std::bind<IMaterialEffect*>(&T::Create, std::placeholders::_1);
		}

		template<class T>
		static void RegisterShaderType()
		{
			mShadersFactory[T::GetClassName()] = std::bind<IShaderProgram*>(&T::Create);
		}

		template<class T>
		static void RegisterRendererType()
		{
			mRenderersFactory[T::GetClassName()] = std::bind<IRenderer*>(&T::Create, std::placeholders::_1, std::placeholders::_2);
		}

		template<class T>
		static void RegisterCameraType()
		{
			mCamerasFactory[T::GetClassName()] = std::bind<ICamera*>(&T::Create, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}

		template<class T>
		static void RegisterComponentType()
		{
			mComponentsFactory[T::GetClassName()] = std::bind<IComponent*>(&T::Create, std::placeholders::_1);
		}

		template<class T>
		static void RegisterGameEntityType()
		{
			mGameEntitiesFactory[T::GetClassName()] = std::bind<std::shared_ptr<IGameEntity>>(&T::Create);
		}

		template<class T>
		static void RegisterGameEvent()
		{
			mGameEventsFactory[T::GetClassName()] = std::bind<std::shared_ptr<GameEvent>>(&T::Create);
		}

		static std::shared_ptr<IGameEntity> CreateEntity(const std::string& name);
		static IMaterialEffect* CreateMaterialEffect(const std::string& name, IMaterial* material);
		static ICamera* CreateOrthogonalCamera(const std::string& name, float screenWidth, float screenHeight, float nearPlane, float farPlane);
		static ICamera* CreatePerspectiveCamera(const std::string& name, float fov, float aspectRatio, float nearPlane, float farPlane);
		static IRenderer* CreateRenderer(const std::string& name, Model* model, IMaterial* material);
		static IShaderProgram* CreateShader(const std::string& name);
		static IComponent* CreateComponent(const std::string& name, std::shared_ptr<IGameEntity> entity);
		static std::shared_ptr<GameEvent> CreateGameEvent(const std::string& name);

	private:
		static std::map<std::string, IFactory*> mFactories;
		static std::map<std::string, RendererCreatorFunction> mRenderersFactory;
		static std::map<std::string, CameraCreatorFunction> mCamerasFactory;
		static std::map<std::string, ShaderCreatorFunction> mShadersFactory;
		static std::map<std::string, MaterialEffectCreatorFunction> mMaterialEffectFactory;
		static std::map<std::string, ComponentCreatorFunction> mComponentsFactory;
		static std::map<std::string, GameEntityCreatorFunction> mGameEntitiesFactory;
		static std::map<std::string, GameEventCreatorFunction> mGameEventsFactory;
};

