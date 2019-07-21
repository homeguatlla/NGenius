#pragma once
#include "../GameEntity.h"


class Light : public GameEntity<Light>
{
public:
	explicit Light(const glm::vec3& position, const glm::vec3& color, IRenderer* renderer);
	explicit Light(const glm::vec3& position, const glm::vec3& color);
	~Light();

	Light* DoClone() const override { return nullptr; }

	const glm::vec3 GetColor() const;
	const glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& position);
	void SetColor(const glm::vec3& color);

private:
	glm::vec3 mColor;
};

