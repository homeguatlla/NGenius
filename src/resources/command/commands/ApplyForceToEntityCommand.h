#pragma once
#include "../ICommand.h"

class ApplyForceToEntityCommand : public ICommand
{
public:
	ApplyForceToEntityCommand(std::shared_ptr<IGameEntity> entity);
	~ApplyForceToEntityCommand() = default;

	void Execute() override;
	bool HasFinished() const override;
	void Update(float elapsedTime) override;
};

