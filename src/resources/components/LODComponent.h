#pragma once
#include "IComponent.h"
#include <vector>

class ICamera;
class IRenderer;

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

public:
	LODComponent(const ICamera* camera);
	~LODComponent();

	IComponent* DoClone() const override;
	void UpdateInternal(float elapsedTime) override;

	void AddLevelOfDetail(IRenderer* renderer, float distance);
};

