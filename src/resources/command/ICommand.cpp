#include "stdafx.h"
#include "ICommand.h"


ICommand::ICommand(IGameEntity* entity) : mEntity(entity)
{
}


ICommand::~ICommand()
{
}
