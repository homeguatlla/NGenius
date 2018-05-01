#pragma once
class GameEntity;

class IComponent
{
	bool mIsEnabled;

protected:
	GameEntity* mParent;

public:
	IComponent();
	virtual ~IComponent();

	void SetParent(GameEntity* parent);
	bool IsEnabled() const;
	void SetEnabled(bool enabled);

	void Update(float elapsedTime);
	virtual void Init() {};

	IComponent* Clone() const;
private:
	virtual void UpdateInternal(float elapsedTime) {};
protected:
	virtual IComponent* DoClone() const = 0;
};

