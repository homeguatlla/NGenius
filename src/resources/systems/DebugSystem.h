#pragma once
#include "ISystem.h"
#include "../../input/IInputListener.h"
#include <vector>

class GameEntity;
class InputHandler;
class RenderSystem;

class DebugSystem : public IInputListener, public ISystem
{
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

	void Update(float elapsedTime) override;
	bool HasToBeRegisteredToGameScene() const override;

	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) {};
	void OnMouseButton(int button, int action, int mods) {};
	void OnMouseCursorPos(double x, double y) {};

	void SetDebugModeEnabled(bool enable);
	bool IsDebugModeEnabled() const;

private:
	
	bool HasSpecificComponents(const GameEntity* entity) const;

	// Heredado vía ISystem
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor & guest) override;
};

