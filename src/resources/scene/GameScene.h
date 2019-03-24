#pragma once

#include <vector>

#include "../../visitor/BaseVisitable.h"
#include "../../AABB.h"
#include "../../utils/serializer/ISerializable.h"

class GameEntity;
class IGameSceneListener;
class RenderSystem;

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
	void ReleaseEntities(std::vector<GameEntity*>* entities);
	void RemoveEntities();
	void AddNewEntities();
	void NotifyEntityAdded(GameEntity* entity);
	void NotifyEntityRemoved(GameEntity* entity);
};

