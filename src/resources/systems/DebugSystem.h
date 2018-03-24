#pragma once
#include "../../input/IInputListener.h"
#include <vector>

class GameEntity;
class InputHandler;
class RenderSystem;

class DebugSystem : public IInputListener
{
	std::vector<GameEntity*> mEntities;
	typedef std::vector<GameEntity*>::iterator GameEntitiesIterator;


	bool mIsBoundingBoxVisible;
	InputHandler* mInputHandler;
	RenderSystem* mRenderSystem;

public:

	DebugSystem(RenderSystem* renderSystem, InputHandler* inputHandler);
	~DebugSystem();

	void Update(float elapsedTime);

	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) {};
	void OnMouseButton(int button, int action, int mods) {};
	void OnMouseCursorPos(double x, double y) {};

	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

	bool HasDebugComponents(GameEntity* entity) const;
};

