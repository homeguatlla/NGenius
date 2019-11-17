#pragma once

#include "../utils/serializer/ISerializable.h"
#include "components/IComponent.h"

#include <map>
#include <typeinfo>
#include <assert.h>
#include <string>

class Transformation;
class IRenderer;
class RenderSystem;
class GameScene;
class NGenius;

class IGameEntity : public core::utils::ISerializable
{
public:
	virtual ~IGameEntity() = default;
	virtual void Init(GameScene* scene, RenderSystem* renderSystem) = 0;
	virtual int GetID() const = 0;
	virtual void SetName(const std::string& name) = 0;
	virtual std::string GetName() const = 0;

	virtual Transformation* GetTransformation() = 0;
	virtual const Transformation* GetTransformation() const = 0;

	virtual IRenderer* GetRenderer() const = 0;
	virtual void SetRenderer(IRenderer* renderer) = 0;

	virtual bool IsEnabled() const = 0;
	virtual void SetEnabled(bool enabled) = 0;

	virtual void Update(float elapsedTime) = 0;
	virtual void Build(NGenius* engine) = 0;

	virtual IGameEntity* DoClone() const = 0;

	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer* source) override = 0;
	virtual void WriteTo(core::utils::ISerializer* destination) override = 0;


	template<typename T> void AddComponent(T* component);
	template<typename T> void AddComponent(const std::type_info* type, T* component);
	template<typename T> void RemoveComponent();
	template<typename T> bool HasComponent() const;
	template<typename T> T* GetComponent();	

protected:
	typedef std::map<const std::type_info*, IComponent*> ComponentsMap;
	typedef ComponentsMap::iterator IComponentsIterator;
	ComponentsMap mComponents;
};

template<typename T> 
void IGameEntity::AddComponent(T* component)
{
	static_assert(std::is_base_of<IComponent, T>::value, "The type must inherit from IComponent");
	assert(component != nullptr);
	if (mComponents.count(&typeid(T)) == 0)
	{
		component->SetParent(this);
		mComponents[&typeid(T)] = component;
	}
	else
	{
		delete component;
	}
}

template<typename T> 
void IGameEntity::AddComponent(const std::type_info* type, T* component)
{
	static_assert(std::is_base_of<IComponent, T>::value, "The type must inherit from IComponent");
	assert(component != nullptr);
	if (mComponents.count(type) == 0)
	{
		component->SetParent(this);
		mComponents[type] = component;
	}
	else
	{
		delete component;
	}
}

template<typename T> void IGameEntity::RemoveComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "The type must inherit from IComponent");
	if (HasComponent<T>())
	{
		T* component = static_cast<T*>(mComponents[&typeid(T)]);
		mComponents.erase(&typeid(T));
		delete component;
	}
}

template<typename T> bool IGameEntity::HasComponent() const
{
	static_assert(std::is_base_of<IComponent, T>::value, "The type must inherit from IComponent");
	return mComponents.count(&typeid(T)) != 0;
}

template<typename T> T* IGameEntity::GetComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "The type must inherit from IComponent");
	if (HasComponent<T>())
	{
		return static_cast<T*>(mComponents[&typeid(T)]);
	}
	else
	{
		return nullptr;
	}
}