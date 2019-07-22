#pragma once
#include "IComponent.h"
#include "../../input/IInputListener.h"

#include <queue>
#include <vector>
#include <string>

class GameEvent;
class InputConverter;
class IGameEntity;

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
	const GameEvent* ConvertMouse(void* data) const;

	static std::string GetClassName() { return std::string("InputComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	InputComponent* DoClone() const override;
};

