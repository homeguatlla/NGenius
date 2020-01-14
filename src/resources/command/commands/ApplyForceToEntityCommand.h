#pragma once
#include "../ICommand.h"

class ApplyForceToEntityCommand : public ICommand
{
public:
	ApplyForceToEntityCommand(IGameEntity* entity);
	~ApplyForceToEntityCommand();

	void Execute() override;
	bool HasFinished() const override;
	void Update(float elapsedTime) override;
};

