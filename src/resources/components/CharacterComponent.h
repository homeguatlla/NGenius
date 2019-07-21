#pragma once
#include "IComponent.h"
#include <queue>
#include<string>


class GameEvent;
class IGameEntity;

class CharacterComponent : public IComponent
{
	std::queue<const GameEvent*> mEvents;

public:
	explicit CharacterComponent();
	virtual ~CharacterComponent();

	void OnCharacterControllerEvent(const GameEvent* gameEvent);
	bool HasEvents() const;
	const GameEvent* ConsumeEvent();

	static std::string GetClassName() { return std::string("CharacterComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	CharacterComponent* DoClone() const override;
};

