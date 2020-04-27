#pragma once
#include "IComponent.h"
#include<string>

class IRenderer;
class IGameEntity;


class DebugComponent : public IComponent
{
	IRenderer* mBoundingBoxRenderer;

public:
	explicit DebugComponent(IRenderer* renderer);
	~DebugComponent();

	void Init(GameScene* scene, RenderSystem* renderSystem) override;
	bool IsBoundingBoxVisible() const;
	void SetBoundingBoxVisibility(bool visible);
	IRenderer* GetBoundingBoxRenderer();
	void ResetRenderer();

	static std::string GetClassName() { return std::string("DebugComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	DebugComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	DebugComponent* DoClone() const override;
};

