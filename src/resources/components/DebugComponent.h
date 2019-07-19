#pragma once
#include "IComponent.h"
#include<string>

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

	static std::string GetClassName() { return std::string("DebugComponent"); }
	static IComponent* Create();

private:
	DebugComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	DebugComponent* DoClone() const override;
};

