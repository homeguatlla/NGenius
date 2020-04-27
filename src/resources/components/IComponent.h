#pragma once
#include "src/utils/serializer/ISerializable.h"
#include <memory>

class IGameEntity;
class RenderSystem;
class GameScene;

class IComponent : public core::utils::ISerializable
{
	bool mIsEnabled;

protected:
	std::shared_ptr<IGameEntity> mParent;

public:
	IComponent();
	virtual ~IComponent();

	void SetParent(std::shared_ptr<IGameEntity> parent);
	bool IsEnabled() const;
	void SetEnabled(bool enabled);

	void Update(float elapsedTime);
	virtual void Init(GameScene* scene, RenderSystem* renderSystem) {};

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

