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
	unsigned int mLastLODIndex;

public:
	LODComponent(const ICamera* camera);
	~LODComponent();

	void UpdateInternal(float elapsedTime) override;

	void AddLevelOfDetail(IRenderer* renderer, float distance);

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	IComponent* DoClone() const override;
};

