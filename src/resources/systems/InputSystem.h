#pragma once
#include "../../input/IInputListener.h"
#include <vector>

class GameEntity;
class InputHandler;

class InputSystem : public IInputListener
{
	std::vector<GameEntity*> mEntities;
	std::vector<GameEntity*> mNewEntitiesToAdd;
	std::vector<GameEntity*> mEntitiesToRemove;
	typedef std::vector<GameEntity*>::iterator GameEntitiesIterator;

public:
	InputSystem(InputHandler* inputHandler);
	~InputSystem();

	void Update(float elapsedTime);

	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override;

	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

	bool HasInputComponents(GameEntity* entity) const;

private:
	void RemoveEntities();
	void AddNewEntities();
};

