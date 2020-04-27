#pragma once
#include "src/utils/fsm/BaseTransition.h"
#include "src/resources/entities/Player/PlayerFsm/PlayerContext.h"
#include "src/resources/entities/Player/PlayerFsm/states/PlayerStates.h"

class PhysicsComponent;

class EnterIdle : public core::utils::FSM::BaseTransition<PlayerState, PlayerContext>
{
public:
	EnterIdle(StatePtr origin, StatePtr destination);
	virtual ~EnterIdle() = default;

	void OnInit() override;
	bool CanPerformTransition() const override;

private:
	std::shared_ptr<PhysicsComponent> mPhysicsComponent;
};
