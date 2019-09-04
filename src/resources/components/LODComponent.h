#pragma once
#include "IComponent.h"
#include <vector>
#include<string>

class ICamera;
class IRenderer;
class IGameEntity;

class LODComponent : public IComponent
{
	using LODData = std::tuple<std::string, std::string, float>;
	struct LOD
	{
		float distance;
		IRenderer* renderer;

		LOD(IRenderer* _renderer, float _distance) : renderer(_renderer), distance(_distance) {}
	};

	const ICamera* mCamera;
	std::vector<LOD*> mLODS;
	const unsigned long MAX_LODS = 5;
	unsigned int mLastLODIndex;
	std::vector<LODData> mLodsToAdd;

public:
	LODComponent(const ICamera* camera);
	~LODComponent();

	void Init(GameScene* scene, RenderSystem* renderSystem) override;
	void UpdateInternal(float elapsedTime) override;
	void AddLevelOfDetail(IRenderer* renderer, float distance);

	static std::string GetClassName() { return std::string("LODComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	LODComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	LODComponent::LODData ReadLODFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination) override;
	IComponent* DoClone() const override;
};

