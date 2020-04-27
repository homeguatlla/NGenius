#pragma once
#include "src/utils/fsm/BaseState.h"
#include "src/resources/entities/Player/PlayerFsm/PlayerContext.h"
#include "src/resources/entities/Player/PlayerFsm/states/PlayerStates.h"

class Idle : public core::utils::FSM::BaseState<PlayerState, PlayerContext>
{
public:
	Idle() = default;
	virtual ~Idle() = default;
	PlayerState GetID() const override { return PlayerState::STATE_IDLE; }

	void OnInit() override;
	void OnEnter(float deltaTime) override;
	void OnExit(float deltaTime) override;
	void OnUpdate(float deltaTime) override;

private:
	void UpdateEvents(float deltaTime);

private:
	std::shared_ptr<Player> mPlayer;
	std::shared_ptr<CharacterComponent> mCharacterComponent;
};

