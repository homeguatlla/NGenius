#pragma once
#include "IComponent.h"

class IRenderer;


class DebugComponent : public IComponent
{
	IRenderer* mBoundingBoxRenderer;

public:
	explicit DebugComponent(IRenderer* renderer);
	~DebugComponent();

	void Init();
	bool IsBoundingBoxVisible() const;
	void SetBoundingBoxVisibility(bool visible);
	IRenderer* GetBoundingBoxRenderer();

private:
	DebugComponent* DoClone() const override;
};

