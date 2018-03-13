#pragma once
#include "IComponent.h"
#include "../../input/IInputListener.h"

#include <queue>
#include <vector>

class InputEvent;
class InputConverter;

class IInputComponent : public IComponent, public IInputListener
{
	typedef std::vector<const InputConverter*>::iterator InputConvertersIterator;
	std::queue<const InputEvent*> mEvents;
	std::vector<const InputConverter*> mConverters;

public:
	explicit IInputComponent();
	virtual ~IInputComponent();

	void OnKey(int key, int action) override;
	void OnMouseScroll(float scroll) override;
	virtual void OnInputEvent(const InputEvent* event) = 0;
	void Update(float elapsedTime);

	void AddConverter(const InputConverter* converter);
	void RemoveConverter(const InputConverter* converter);
};

