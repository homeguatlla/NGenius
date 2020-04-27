#pragma once
#include <memory>

class IGameEntity;

class ICommand
{
protected:
	std::shared_ptr<IGameEntity> mEntity;

public:
	ICommand(std::shared_ptr<IGameEntity> entity);
	virtual ~ICommand() = default;

	virtual void Execute() = 0;
	virtual bool HasFinished() const { return true; }
	virtual void Update(float elapsedTime) {};
};

