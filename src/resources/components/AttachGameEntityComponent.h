#pragma once
#include "IComponent.h"

class GameEntity;

class AttachGameEntityComponent : public IComponent
{
	GameEntity* mGameEntityAttached;

public:
	AttachGameEntityComponent(GameEntity* entity);
	~AttachGameEntityComponent();

	GameEntity* GetGameEntityAttached() const;

private:
	AttachGameEntityComponent* DoClone() const override;
};

