#include "stdafx.h"
#include "EnableDisablePhysicsEngineCommand.h"
#include "src/NGenius.h"

EnableDisablePhysicsEngineCommand::EnableDisablePhysicsEngineCommand(std::shared_ptr<NGenius> engine, bool enable) :
	ICommand(nullptr),
	mIsEnabled(enable),
	mEngine(engine)
{
}

void EnableDisablePhysicsEngineCommand::Execute()
{
	mEngine->EnablePhysicsEngine(mIsEnabled);
}

bool EnableDisablePhysicsEngineCommand::HasFinished() const
{
	return true;
}

void EnableDisablePhysicsEngineCommand::Update(float elapsedTime)
{
}
