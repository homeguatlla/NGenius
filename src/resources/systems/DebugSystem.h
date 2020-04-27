#pragma once
#include "src/resources/scene/IGameSceneListener.h"
#include "src/input/IInputListener.h"
#include <vector>
#include <memory>

class IGameEntity;
class InputHandler;
class NGenius;
class RenderSystem;
class IMaterial;
class Text;

class DebugSystem : public IInputListener, public IGameSceneListener
{
	std::vector<std::shared_ptr<IGameEntity>> mEntities;
	typedef std::vector<std::shared_ptr<IGameEntity>>::iterator GameEntitiesIterator;

	bool mIsDebugModeEnabled;
	bool mIsBoundingBoxVisible;
	bool mIsColliderVisible;
	bool mIsOverdrawEnabled;
	bool mIsWireframeEnabled;
	bool mIsPostProcessEnabled;
	InputHandler* mInputHandler;
	NGenius* mEngine;
	RenderSystem* mRenderSystem;
	bool mIsInitialized;
	std::vector<std::shared_ptr<Text>> mText;
	IMaterial* materialText;
	bool mAreTextVisible;

public:

	DebugSystem(NGenius* engine, RenderSystem* renderSystem, InputHandler* inputHandler);
	~DebugSystem();

	void Init();
	void Start();

	void Update(float elapsedTime);

	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) {};
	void OnMouseButton(int button, int action, int mods) {};
	void OnMouseCursorPos(double x, double y) {};

	void SetDebugModeEnabled(bool enable);
	bool IsDebugModeEnabled() const;

private:
	void AddEntity(std::shared_ptr<IGameEntity> entity);
	void RemoveEntity(std::shared_ptr<IGameEntity> entity);

	bool HasDebugComponents(const std::shared_ptr<IGameEntity> entity) const;

	void CreateStatisticsTexts();
	void UpdateStatitstics();
	void UpdateVisibility();

	void SetTextsVisibility(bool visible);

	void OnGameEntityAdded(std::shared_ptr<IGameEntity> entity) override;
	void OnGameEntityRemoved(std::shared_ptr<IGameEntity> entity) override;
};

