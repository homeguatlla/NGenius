#pragma once
class GameEvent
{
public:
	GameEvent();
	~GameEvent();

	GameEvent* Clone() const;

	template<typename T>
	bool IsOfType() const
	{
		return typeid(T) == typeid(*this);
	}

	virtual void SetFloatValue(float value) {};

private:
	virtual GameEvent* DoClone() const = 0;
};

