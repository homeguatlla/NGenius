#pragma once
#include "src/utils/fsm/BaseState.h"
#include "src/resources/entities/Player/PlayerFsm/PlayerContext.h"
#include "src/resources/entities/Player/PlayerFsm/states/PlayerStates.h"

class OnAir : public core::utils::FSM::BaseState<PlayerState, PlayerContext>
{
public:
	OnAir() = default;
	virtual ~OnAir() = default;
	PlayerState GetID() const override { return PlayerState::STATE_ON_AIR; }
	void OnInit() override;
	void OnEnter(float deltaTime) override;


private:
	std::shared_ptr<Player> mPlayer;
};

