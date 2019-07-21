#pragma once
#include "../../input/IInputListener.h"
#include "../scene/IGameSceneListener.h"
#include <vector>

class IGameEntity;
class InputHandler;

class InputSystem : public IInputListener, public IGameSceneListener
{
	std::vector<IGameEntity*> mEntities;
	typedef std::vector<IGameEntity*>::iterator GameEntitiesIterator;

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

private:
	bool HasInputComponents(const IGameEntity* entity) const;
	void AddEntity(IGameEntity* entity);
	void RemoveEntity(IGameEntity* entity);
	void DispatchEvent(MouseData& data);

	void OnGameEntityAdded(IGameEntity* entity) override;
	void OnGameEntityRemoved(IGameEntity* entity) override;
};

