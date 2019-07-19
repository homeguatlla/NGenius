#pragma once

#include <vector>
#include <string>

#include "../../visitor/BaseVisitable.h"
#include "../../AABB.h"
#include "../../utils/serializer/ISerializable.h"

class GameEntity;
class IGameSceneListener;
class RenderSystem;
class IComponent;

class GameScene : public core::utils::ISerializable, BaseVisitable<>
{
	std::vector<GameEntity*> mEntities;
	std::vector<GameEntity*> mNewEntitiesToAdd;
	std::vector<GameEntity*> mEntitiesToRemove;
	typedef std::vector<GameEntity*>::iterator GameEntitiesIterator;
	std::vector<IGameSceneListener*> mListeners;
	typedef std::vector<IGameSceneListener*>::iterator ListenersIterator;
	std::string mName;
	AABB mAABB;

public:
	GameScene(const std::string& name);
	~GameScene();

	void Start(RenderSystem* renderSystem);

	void Update(float elapsedTime);
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

	void Render(RenderSystem* renderSystem);
	
	unsigned int GetNumberGameEntities() const;
	
	std::vector<GameEntity*>& GetAllGameEntities();

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
	void ReleaseEntities(std::vector<GameEntity*>* entities);
	void RemoveEntities();
	void AddNewEntities();
	void NotifyEntityAdded(GameEntity* entity);
	void NotifyEntityRemoved(GameEntity* entity);

	GameEntity* ReadEntityFrom(core::utils::IDeserializer* source);
	void ReadComponentsFrom(GameEntity* entity, core::utils::IDeserializer* source);
	IComponent* ReadComponentFrom(core::utils::IDeserializer* source);
};

