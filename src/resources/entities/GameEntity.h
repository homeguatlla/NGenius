#pragma once
#include "src/resources/BaseGameEntity.h"


class GameEntity : public BaseGameEntity<GameEntity>
{
public:
	GameEntity() = default;
	explicit GameEntity(Transformation* transformation, IRenderer* renderer);
	explicit GameEntity(Transformation* transformation);
	~GameEntity() = default;

	std::shared_ptr<IGameEntity> DoClone() const override;

	static std::string GetClassName() { return std::string("entity"); }
	static std::shared_ptr<IGameEntity> DoCreate();

private:
	

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

