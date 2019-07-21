#pragma once
#include "../../utils/serializer/ISerializable.h"

class IGameEntity;

class IComponent : public core::utils::ISerializable
{
	bool mIsEnabled;

protected:
	IGameEntity* mParent;

public:
	IComponent();
	virtual ~IComponent();

	void SetParent(IGameEntity* parent);
	bool IsEnabled() const;
	void SetEnabled(bool enabled);

	void Update(float elapsedTime);
	virtual void Init() {};

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;

	IComponent* Clone() const;
private:
	virtual void UpdateInternal(float elapsedTime) {};
protected:
	virtual IComponent* DoClone() const = 0;
	virtual void DoReadFrom(core::utils::IDeserializer* source) = 0;
	virtual void DoWriteTo(core::utils::ISerializer* destination) = 0;
};

