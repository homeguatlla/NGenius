#pragma once
#include "IComponent.h"

class IRenderer;


class DebugComponent : public IComponent
{
	IRenderer* mBoundingBoxRenderer;
	bool mIsBoundingBoxVisible;

public:
	explicit DebugComponent(IRenderer* renderer);
	~DebugComponent();

	DebugComponent* DoClone() const override;
	bool IsBoundingBoxVisible() const;
	void SetBoundingBoxVisibility(bool visible);
	IRenderer* GetBoundingBoxRenderer();

private:
	void UpdateBoundingBox();
};

