#pragma once
#include "../BaseGameEntity.h"


class Light : public BaseGameEntity<Light>
{
public:
	explicit Light(const glm::vec3& position, const glm::vec3& color, IRenderer* renderer);
	explicit Light(const glm::vec3& position, const glm::vec3& color);
	~Light();

	std::shared_ptr<IGameEntity> DoClone() const override { return nullptr; }

	const glm::vec3 GetColor() const;
	const glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& position);
	void SetColor(const glm::vec3& color);

private:
	glm::vec3 mColor;

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

