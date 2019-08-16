#pragma once
#include "IComponent.h"
#include <queue>
#include<string>
#include<memory>


class GameEvent;
class IGameEntity;

class CharacterComponent : public IComponent
{
	std::queue<std::shared_ptr<const GameEvent>> mEvents;

public:
	explicit CharacterComponent();
	virtual ~CharacterComponent();

	void OnCharacterControllerEvent(std::shared_ptr<const GameEvent> gameEvent);
	bool HasEvents() const;
	std::shared_ptr<const GameEvent> ConsumeEvent();

	static std::string GetClassName() { return std::string("CharacterComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	CharacterComponent* DoClone() const override;
};

