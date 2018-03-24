#pragma once
#include "IComponent.h"


class DebugComponent : public IComponent
{
	bool mIsBoundingBoxVisible;

public:
	explicit DebugComponent();
	~DebugComponent();

	DebugComponent* DoClone() const override;
	bool IsBoundingBoxVisible() const;
	void SetBoundingBoxVisibility(bool visible);
};

