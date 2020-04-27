#pragma once

#include <vector>
#include <string>
#include <memory>

#include "src/visitor/BaseVisitable.h"
#include "src/AABB.h"
#include "src/utils/serializer/ISerializable.h"

class IGameEntity;
class IGameSceneListener;
class NGenius;
class IComponent;
class RenderSystem;
class Terrain;

class GameScene : public core::utils::ISerializable, BaseVisitable<>
{
	std::vector<std::shared_ptr<IGameEntity>> mEntities;
	std::vector<std::shared_ptr<IGameEntity>> mNewEntitiesToAdd;
	std::vector<std::shared_ptr<IGameEntity>> mEntitiesToRemove;
	typedef std::vector<std::shared_ptr<IGameEntity>>::iterator GameEntitiesIterator;
	std::vector<IGameSceneListener*> mListeners;
	typedef std::vector<IGameSceneListener*>::iterator ListenersIterator;
	std::string mName;
	AABB mAABB;
	NGenius* mEngine;
	RenderSystem* mRenderSystem;
	
	//To set Entities on ground
	std::shared_ptr<Terrain> mGround;

public:
	GameScene(const std::string& name, NGenius* engine, RenderSystem* renderSystem);
	~GameScene();

	void Start();

	void Update(float elapsedTime);
	void AddEntity(std::shared_ptr<IGameEntity> entity);
	void RemoveEntity(std::shared_ptr<IGameEntity> entity);
	std::shared_ptr<IGameEntity> GetGameEntity(const std::string& name);
	std::shared_ptr<IGameEntity> GetGameEntity(int id);

	void Render(RenderSystem* renderSystem);
	
	unsigned int GetNumberGameEntities() const;
	
	std::vector<std::shared_ptr<IGameEntity>>& GetAllGameEntities();

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
	void RemoveEntities();
	void AddNewEntities();
	void NotifyEntityAdded(std::shared_ptr<IGameEntity> entity);
	void NotifyEntityRemoved(std::shared_ptr<IGameEntity> entity);

	void SetEntitiesOnGround();
	void SetEntityOnGround(std::shared_ptr<IGameEntity> entity);
	bool CheckIsGround(std::shared_ptr<IGameEntity> entity);

	std::shared_ptr<IGameEntity> ReadEntityFrom(core::utils::IDeserializer* source);
	void ReadComponentsFrom(std::shared_ptr<IGameEntity> entity, core::utils::IDeserializer* source);
	void ReadComponentFrom(std::shared_ptr<IGameEntity> entity, core::utils::IDeserializer* source);
};

