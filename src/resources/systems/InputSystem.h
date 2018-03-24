#pragma once
#include "../../input/IInputListener.h"
#include <vector>

class GameEntity;
class InputHandler;

class InputSystem : public IInputListener
{
	std::vector<GameEntity*> mEntities;
	typedef std::vector<GameEntity*>::iterator GameEntitiesIterator;

	InputHandler* mInputHandler;

public:
	typedef struct MouseData
	{
		int mButton;
		int mAction;
		int mMods;
		double mCursorX;
		double mCursorY;
		float mScroll;
		bool mIsCursorMoved;
		MouseData(int button, float scroll) :	mButton(button), mAction(-1), mMods(-1), mCursorX(0.0), mCursorY(0.0), 
												mScroll(scroll), mIsCursorMoved(false) {};
		MouseData(double x, double y) : mButton(-1), mAction(-1), mMods(-1), mCursorX(x), mCursorY(y), mScroll(0.0), 
										mIsCursorMoved(true) {};
		MouseData(int button, int action, int mods) :	mButton(button), mAction(action), mMods(mods), 
														mCursorX(0.0), mCursorY(0.0), mScroll(0.0), mIsCursorMoved(false) {};
	} MouseData;

	InputSystem(InputHandler* inputHandler);
	~InputSystem();

	void Update(float elapsedTime);

	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnMouseCursorPos(double x, double y) override;

	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

	bool HasInputComponents(GameEntity* entity) const;

private:
	void DispatchEvent(MouseData& data);
};

