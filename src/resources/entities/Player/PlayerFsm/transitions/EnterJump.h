#pragma once
#include "src/utils/fsm/BaseTransition.h"
#include "src/resources/entities/Player/PlayerFsm/PlayerContext.h"
#include "src/resources/entities/Player/PlayerFsm/states/PlayerStates.h"

class Player;

class EnterJump : public core::utils::FSM::BaseTransition<PlayerState, PlayerContext>
{
public:
	EnterJump(StatePtr origin, StatePtr destination);
	virtual ~EnterJump() = default;

	void OnInit() override;
	bool CanPerformTransition() const override;

private:
	std::shared_ptr<Player> mPlayer;
};