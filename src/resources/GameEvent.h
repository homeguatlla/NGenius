#pragma once
class GameEvent
{
public:
	GameEvent();
	~GameEvent();

	GameEvent* Clone(const void* data) const;

	template<typename T>
	bool IsOfType() const
	{
		return typeid(T) == typeid(*this);
	}

private:
	virtual GameEvent* DoClone(const void* data) const = 0;
};

