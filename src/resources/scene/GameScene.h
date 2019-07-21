#pragma once

#include <vector>
#include <string>

#include "../../visitor/BaseVisitable.h"
#include "../../AABB.h"
#include "../../utils/serializer/ISerializable.h"

class IGameEntity;
class IGameSceneListener;
class RenderSystem;
class IComponent;

class GameScene : public core::utils::ISerializable, BaseVisitable<>
{
	std::vector<IGameEntity*> mEntities;
	std::vector<IGameEntity*> mNewEntitiesToAdd;
	std::vector<IGameEntity*> mEntitiesToRemove;
	typedef std::vector<IGameEntity*>::iterator GameEntitiesIterator;
	std::vector<IGameSceneListener*> mListeners;
	typedef std::vector<IGameSceneListener*>::iterator ListenersIterator;
	std::string mName;
	AABB mAABB;

public:
	GameScene(const std::string& name);
	~GameScene();

	void Start(RenderSystem* renderSystem);

	void Update(float elapsedTime);
	void AddEntity(IGameEntity* entity);
	void RemoveEntity(IGameEntity* entity);

	void Render(RenderSystem* renderSystem);
	
	unsigned int GetNumberGameEntities() const;
	
	std::vector<IGameEntity*>& GetAllGameEntities();

	const AABB& GetAABB() const;

	void RegisterGameSceneListener(IGameSceneListener* listener);
	void UnRegisterGameSceneListener(IGameSceneListener* listener);

	void SaveToFile();
	
	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;

	BaseVisitable<>::ReturnType Accept(BaseVisitor& guest) override;

private:
	void Release();
	void ReleaseEntities(std::vector<IGameEntity*>* entities);
	void RemoveEntities();
	void AddNewEntities();
	void NotifyEntityAdded(IGameEntity* entity);
	void NotifyEntityRemoved(IGameEntity* entity);

	IGameEntity* ReadEntityFrom(core::utils::IDeserializer* source);
	void ReadComponentsFrom(IGameEntity* entity, core::utils::IDeserializer* source);
	void ReadComponentFrom(IGameEntity* entity, core::utils::IDeserializer* source);
};

