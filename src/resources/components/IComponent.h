#pragma once
class GameEntity;

class IComponent
{
protected:
	GameEntity* mParent;

public:
	IComponent();
	virtual ~IComponent();

	void SetParent(GameEntity* parent);
	virtual void Update(float elapsedTime) {};
	virtual void Init() {};

	IComponent* Clone() const;
protected:
	virtual IComponent* DoClone() const = 0;
};

