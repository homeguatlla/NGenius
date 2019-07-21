#pragma once

class IGameEntity;

class ICommand
{
protected:
	IGameEntity* mEntity;

public:
	ICommand(IGameEntity* entity);
	virtual ~ICommand();

	virtual void Execute() = 0;
	virtual bool HasFinished() const { return true; }
	virtual void Update(float elapsedTime) {};
};

