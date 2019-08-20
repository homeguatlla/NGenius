#pragma once
#include "../BaseGameEntity.h"


class GameEntity : public BaseGameEntity<GameEntity>
{
public:
	explicit GameEntity(Transformation* transformation, IRenderer* renderer);
	explicit GameEntity(Transformation* transformation);
	~GameEntity() = default;

	GameEntity* DoClone() const override { return nullptr; }

	static std::string GetClassName() { return std::string("entity"); }
	static IGameEntity* DoCreate();

private:
	GameEntity() = default;

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

