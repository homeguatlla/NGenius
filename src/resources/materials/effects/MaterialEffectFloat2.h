#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class MaterialEffectFloat2 : public IMaterialEffect
{
	glm::vec2 mValue;

public:
	MaterialEffectFloat2(const glm::vec2& value);
	virtual ~MaterialEffectFloat2();

	void SetValue(const glm::vec2& value);
	const glm::vec2 GetValue() const;

	MaterialEffectFloat2* DoClone() const override;
};

