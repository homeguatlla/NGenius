#include "stdafx.h"
#include "Light.h"


Light::Light(const glm::vec3& position, const glm::vec3& color, IRenderer* renderer) :
	BaseGameEntity(new Transformation(position, glm::vec3(0.0f), glm::vec3(1.0f)), renderer),
mColor(color)
{
}

Light::Light(const glm::vec3& position, const glm::vec3& color) : 
Light(position, color, nullptr)
{
	
}

Light::~Light()
{
}

const glm::vec3 Light::GetColor() const
{
	return mColor;
}

const glm::vec3 Light::GetPosition() const
{
	return GetTransformation()->GetPosition();
}

void Light::SetPosition(const glm::vec3& position)
{
	GetTransformation()->SetPosition(position);
}

void Light::SetColor(const glm::vec3& color)
{
	mColor = color;
}

void Light::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}
