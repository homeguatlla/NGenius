#pragma once
#include "Transformation.h"
#include<map>
#include<typeinfo>
#include "components/IComponent.h"
#include "../utils/serializer/ISerializable.h"
class IRenderer;

class GameEntity : public core::utils::ISerializable
{
	typedef std::map<const std::type_info*, IComponent*> ComponentsMap;
	typedef ComponentsMap::iterator IComponentsIterator;

	Transformation* mTransformation;
	IRenderer* mRenderer;
	ComponentsMap mComponents;
	bool mIsEnabled;

protected:
	
	virtual GameEntity* DoClone() const;

public:
	explicit GameEntity(Transformation* transformation, IRenderer* renderer);
	explicit GameEntity(Transformation* transformation);
	virtual ~GameEntity();

	void Init();
	IRenderer* GetRenderer() const;
	void SetRenderer(IRenderer* renderer);
	Transformation* GetTransformation();
	const Transformation* GetTransformation() const;
	GameEntity* Clone();

	bool IsEnabled() const;
	void SetEnabled(bool enabled);

	virtual void Update(float elapsedTime);

	template<typename T> void AddComponent(T* component)
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

	template<typename T> void RemoveComponent()
	{
		static_assert(std::is_base_of<IComponent, T>::value, "The type must inherit from IComponent");
		if (HasComponent<T>())
		{ 
			T* component = static_cast<T*>(mComponents[&typeid(T)]);
			mComponents.erase(&typeid(T));
			delete component;
		}
	}

	template<typename T> bool HasComponent() const
	{
		static_assert(std::is_base_of<IComponent, T>::value, "The type must inherit from IComponent");
		return mComponents.count(&typeid(T)) != 0;
	}

	template<typename T> T* GetComponent()
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

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;

private:
	template<typename T> void AddComponent(const std::type_info* type, T* component)
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
};

