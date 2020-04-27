#include "stdafx.h"
#include "BillboardComponent.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"
#include "../IGameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Memory.h"

BillboardComponent::BillboardComponent(const ICamera* camera) :
mCamera(camera)
{
}


BillboardComponent::~BillboardComponent()
{
}

BillboardComponent* BillboardComponent::DoClone() const
{
	return DBG_NEW BillboardComponent(*this);
}

void BillboardComponent::UpdateInternal(float elapsedTime)
{
	const glm::mat4 viewMatrix = const_cast<ICamera*>(mCamera)->GetViewMatrix();
	glm::mat4 modelMatrix = mParent->GetTransformation()->GetModelMatrix();
	glm::vec3 scale = mParent->GetTransformation()->GetScale();
	float angleZ = mParent->GetTransformation()->GetRotation().z;
	ModifyModelMatrixToAvoidRotations(viewMatrix, scale, angleZ, modelMatrix);
}

IComponent* BillboardComponent::Create(std::shared_ptr<IGameEntity> entity)
{
	BillboardComponent* component = DBG_NEW BillboardComponent();

	entity->AddComponent(component);

	return component;
}

void BillboardComponent::ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix)
{
	modelMatrix[0][0] = viewMatrix[0][0];
	modelMatrix[0][1] = viewMatrix[1][0];
	modelMatrix[0][2] = viewMatrix[2][0];

	modelMatrix[1][0] = viewMatrix[0][1];
	modelMatrix[1][1] = viewMatrix[1][1];
	modelMatrix[1][2] = viewMatrix[2][1];

	modelMatrix[2][0] = viewMatrix[0][2];
	modelMatrix[2][1] = viewMatrix[1][2];
	modelMatrix[2][2] = viewMatrix[2][2];

	modelMatrix = glm::rotate(modelMatrix, angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);
}

void BillboardComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void BillboardComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("billboard_component"));
	destination->WriteParameter(std::string("camera_name"), mCamera->GetName());
}