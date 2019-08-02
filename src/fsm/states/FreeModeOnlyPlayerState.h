#pragma once
#include "../../utils/fsm/BaseState.h"
#include "../FSMContext.h"
#include "StateTypes.h"
#include "FreeModeOnlyCameraState.h"

#include <glm/glm.hpp>

class ICamera;
class Text;
class IGameEntity;

class FreeModeOnlyPlayerState : public FreeModeOnlyCameraState
{
public:
	FreeModeOnlyPlayerState();
	virtual ~FreeModeOnlyPlayerState();

	NGeniusState GetID() const override { return NGeniusState::STATE_FREE_PLAYER;  }
	void OnEnter(float deltaTime) override;

	// Heredado vía IInputListener
	void OnKey(int key, int action) override;
	void OnMouseCursorPos(double x, double y) override;
};

