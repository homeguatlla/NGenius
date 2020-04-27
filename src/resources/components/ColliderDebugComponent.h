#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include<string>

class IRenderer;
class IGameEntity;
class Model;
class IMaterial;


class ColliderDebugComponent : public IComponent
{
	IRenderer* mBoundingVolumeRenderer;

public:
	explicit ColliderDebugComponent(IRenderer* renderer);
	~ColliderDebugComponent();

	void Init(GameScene* scene, RenderSystem* renderSystem) override;
	bool IsBoundingVolumeVisible() const;
	void SetBoundingVolumeVisibility(bool visible);
	IRenderer* GetBoundingVolumeRenderer();
	void ResetRenderer();

	static std::string GetClassName() { return std::string("ColliderDebugComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

private:
	ColliderDebugComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	ColliderDebugComponent* DoClone() const override;
	std::string GetColliderModelNameAndSize(RenderSystem* renderSystem, glm::vec3& position, glm::vec3& size);
	void CreateBoundingVolumeRenderer(const std::string& modelName, RenderSystem* renderSystem);
};

