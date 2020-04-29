#pragma once
#include "src/utils/fsm/BaseState.h"
#include "src/resources/entities/Player/PlayerFsm/PlayerContext.h"
#include "src/resources/entities/Player/PlayerFsm/states/PlayerStates.h"

class Walk : public core::utils::FSM::BaseState<PlayerState, PlayerContext>
{
public:
	Walk() = default;
	virtual ~Walk() = default;
	PlayerState GetID() const override { return PlayerState::STATE_WALKING; }
	void OnInit() override;
	void OnEnter(float deltaTime) override;
	void OnUpdate(float deltaTime) override;

private:
	std::shared_ptr<Player> mPlayer;
};

