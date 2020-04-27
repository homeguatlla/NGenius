#include "stdafx.h"
#include "ICommand.h"


ICommand::ICommand(std::shared_ptr<IGameEntity> entity) : mEntity(entity)
{
}
