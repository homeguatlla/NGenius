#pragma once
#include "src/utils/fsm/BaseTransition.h"
#include "src/resources/entities/Player/PlayerFsm/PlayerContext.h"
#include "src/resources/entities/Player/PlayerFsm/states/PlayerStates.h"

class Player;
class CollisionComponent;

class EnterRun : public core::utils::FSM::BaseTransition<PlayerState, PlayerContext>
{
public:
	EnterRun(StatePtr origin, StatePtr destination);
	virtual ~EnterRun() = default;

	void OnInit() override;
	bool CanPerformTransition() const override;

private:
	std::shared_ptr<CollisionComponent> mCollisionComponent;
	std::shared_ptr<Player> mPlayer;
};