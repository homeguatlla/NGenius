#pragma once
#include <memory>


class GameEvent
{
public:
	GameEvent();
	~GameEvent();

	std::shared_ptr<GameEvent> Clone(const void* data) const;

	template<typename T>
	bool IsOfType() const
	{
		return typeid(T) == typeid(*this);
	}

private:
	virtual std::shared_ptr<GameEvent> DoClone(const void* data) const = 0;
};

