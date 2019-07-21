#pragma once
#include "../scene/IGameSceneListener.h"
#include "../../input/IInputListener.h"
#include <vector>

class IGameEntity;
class InputHandler;
class NGenius;
class RenderSystem;

class DebugSystem : public IInputListener, public IGameSceneListener
{
	std::vector<IGameEntity*> mEntities;
	typedef std::vector<IGameEntity*>::iterator GameEntitiesIterator;

	bool mIsDebugModeEnabled;
	bool mIsBoundingBoxVisible;
	bool mIsOverdrawEnabled;
	bool mIsWireframeEnabled;
	bool mIsPostProcessEnabled;
	InputHandler* mInputHandler;
	NGenius* mEngine;
	RenderSystem* mRenderSystem;
	bool mIsInitialized;

public:

	DebugSystem(NGenius* engine, RenderSystem* renderSystem, InputHandler* inputHandler);
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
	void AddEntity(IGameEntity* entity);
	void RemoveEntity(IGameEntity* entity);

	bool HasDebugComponents(const IGameEntity* entity) const;

	void OnGameEntityAdded(IGameEntity* entity) override;
	void OnGameEntityRemoved(IGameEntity* entity) override;
};

