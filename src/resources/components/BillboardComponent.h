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

	BillboardComponent* DoClone() const override;
	void Update(float elapsedTime) override;

private:
	void ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix);
};

