#pragma once
#include "IComponent.h"
#include "../../input/IInputListener.h"

#include <queue>
#include <vector>
#include <string>
#include <memory>

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

	std::shared_ptr<const GameEvent> ConvertKey(int key, int action) const;
	std::shared_ptr<const GameEvent> ConvertMouse(void* data) const;

	static std::string GetClassName() { return std::string("InputComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	InputConverter* ReadConverterFrom(core::utils::IDeserializer* source);
	InputComponent* DoClone() const override;
};

