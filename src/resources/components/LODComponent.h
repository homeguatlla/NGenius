#pragma once
#include "IComponent.h"
#include <vector>
#include<string>

class ICamera;
class IRenderer;
class IGameEntity;

class LODComponent : public IComponent
{
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

public:
	LODComponent(const ICamera* camera);
	~LODComponent();

	void UpdateInternal(float elapsedTime) override;
	void AddLevelOfDetail(IRenderer* renderer, float distance);

	static std::string GetClassName() { return std::string("LODComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	LODComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	IComponent* DoClone() const override;
};

