#pragma once
#include "Transformation.h"
#include<map>
#include<typeinfo>
#include "components/IComponent.h"

class IRenderer;

class GameEntity
{
	Transformation* mTransformation;
	IRenderer* mRenderer;
	typedef std::map<const std::type_info*, IComponent*> ComponentsMap;
	typedef ComponentsMap::iterator IComponentsIterator;
	ComponentsMap mComponents;
	bool mIsEnabled;

protected:
	
	virtual GameEntity* DoClone() const;

public:
	explicit GameEntity(Transformation* transformation, IRenderer* renderer);
	explicit GameEntity(Transformation* transformation);
	virtual ~GameEntity();

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
		if (HasComponent<T>())
		{ 
			T* component = static_cast<T*>(mComponents[&typeid(T)]);
			mComponents.erase(&typeid(T));
			delete component;
		}
	}

	template<typename T> bool HasComponent()
	{
		return mComponents.count(&typeid(T)) != 0;
	}

	template<typename T> T* GetComponent()
	{
		if (HasComponent<T>())
		{
			return static_cast<T*>(mComponents[&typeid(T)]);
		}
		else
		{
			return nullptr;
		}
	}

private:
	template<typename T> void AddComponent(const std::type_info* type, T* component)
	{
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

