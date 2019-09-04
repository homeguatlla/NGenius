#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include<string>

class ICamera;
class IGameEntity;

class BillboardComponent : public IComponent
{
	const ICamera* mCamera;

public:
	BillboardComponent(const ICamera* camera);
	~BillboardComponent();

	BillboardComponent* DoClone() const override;
	void UpdateInternal(float elapsedTime) override;

	static std::string GetClassName() { return std::string("BillboardComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	BillboardComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	void ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix);
};

