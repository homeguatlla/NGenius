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
	typedef struct MouseData
	{
		int mButton;
		int mAction;
		float mScroll;
		MouseData(int button, float scroll) : mButton(button), mAction(-1), mScroll(scroll) {};
	} MouseData;

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

