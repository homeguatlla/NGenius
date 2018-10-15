#pragma once
#include "../scene/IGameSceneListener.h"
#include "../../input/IInputListener.h"
#include <vector>

class GameEntity;
class InputHandler;
class RenderSystem;

class DebugSystem : public IInputListener, public IGameSceneListener
{
	std::vector<GameEntity*> mEntities;
	typedef std::vector<GameEntity*>::iterator GameEntitiesIterator;

	bool mIsDebugModeEnabled;
	bool mIsBoundingBoxVisible;
	bool mIsOverdrawEnabled;
	bool mIsWireframeEnabled;
	bool mIsPostProcessEnabled;
	InputHandler* mInputHandler;
	RenderSystem* mRenderSystem;
	bool mIsInitialized;

public:

	DebugSystem(RenderSystem* renderSystem, InputHandler* inputHandler);
	~DebugSystem();

	void Start();

	void Update(float elapsedTime);

	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) {};
	void OnMouseButton(int button, int action, int mods) {};
	void OnMouseCursorPos(double x, double y) {};

	void SetDebugModeEnabled(bool enable);
	bool IsDebugModeEnabled() const;

private:
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

	bool HasDebugComponents(const GameEntity* entity) const;

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
};

