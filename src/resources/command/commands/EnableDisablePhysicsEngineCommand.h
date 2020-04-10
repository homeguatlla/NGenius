#pragma once
#include "src/resources/command/ICommand.h"
#include <memory>

class NGenius;

class EnableDisablePhysicsEngineCommand : public ICommand
{
public:
	EnableDisablePhysicsEngineCommand(std::shared_ptr<NGenius> engine, bool enable);
	~EnableDisablePhysicsEngineCommand() = default;

	void Execute() override;
	bool HasFinished() const override;
	void Update(float elapsedTime) override;

private:
	bool mIsEnabled;
	std::shared_ptr<NGenius> mEngine;
};

