#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>

class ICamera;

class BillboardComponent : public IComponent
{
	const ICamera* mCamera;

public:
	BillboardComponent(const ICamera* camera);
	~BillboardComponent();

	void UpdateInternal(float elapsedTime) override;

private:
	BillboardComponent* DoClone() const override;

	void ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix);
};

