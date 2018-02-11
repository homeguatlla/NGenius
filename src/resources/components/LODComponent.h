#pragma once
#include "IComponent.h"
#include <vector>

class ICamera;
class IRenderer_;

class LODComponent : public IComponent
{
	struct LOD
	{
		float distance;
		IRenderer_* renderer;

		LOD(IRenderer_* _renderer, float _distance) : renderer(_renderer), distance(_distance) {}
	};

	const ICamera* mCamera;
	std::vector<LOD*> mLODS;
	const unsigned long MAX_LODS = 5;

public:
	LODComponent(const ICamera* camera);
	~LODComponent();

	IComponent* DoClone() const override;
	void Update(float elapsedTime) override;

	void AddLevelOfDetail(IRenderer_* renderer, float distance);
};

