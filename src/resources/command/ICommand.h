#pragma once

class GameEntity;

class ICommand
{
protected:
	GameEntity* mEntity;

public:
	ICommand(GameEntity* entity);
	virtual ~ICommand();

	virtual void Execute() = 0;
	virtual bool HasFinished() const { return true; }
	virtual void Update(float elapsedTime) {};
};

