#pragma once
#include "IComponent.h"
#include "../../input/IInputListener.h"

#include <queue>
#include <vector>

class GameEvent;
class InputConverter;

class InputComponent : public IComponent
{
	typedef std::vector<const InputConverter*>::iterator InputConvertersIterator;
	std::vector<const InputConverter*> mConverters;

public:
	explicit InputComponent();
	virtual ~InputComponent();

	void AddConverter(const InputConverter* converter);
	void RemoveConverter(const InputConverter* converter);

	const GameEvent* ConvertKey(int key, int action) const;
	const GameEvent* ConvertMouse(int button, float value) const;

	InputComponent* DoClone() const override;
};

