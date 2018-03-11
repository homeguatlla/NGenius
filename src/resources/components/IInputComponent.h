#pragma once
#include "IComponent.h"
#include "../../input/IInputListener.h"

class IInputComponent : public IComponent, public IInputListener
{
public:
	explicit IInputComponent();
	virtual ~IInputComponent();

	void OnKey(int key, int action) override {}
	void OnMouseScroll(float scroll) override {}
	virtual void Update(float elapsedTime) = 0;
};

