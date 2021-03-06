#include "stdafx.h"
#include "IComponent.h"
#include "../IGameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"

#include <typeinfo>
#include <assert.h>

IComponent::IComponent() : mIsEnabled(true), mParent(nullptr)
{
}


IComponent::~IComponent()
{
}

void IComponent::SetParent(std::shared_ptr<IGameEntity> parent)
{
	mParent = parent;
}

void IComponent::Update(float elapsedTime)
{
	if (IsEnabled())
	{
		UpdateInternal(elapsedTime);
	}
}

bool IComponent::IsEnabled() const
{
	return mIsEnabled;
}

void IComponent::SetEnabled(bool enabled)
{
	mIsEnabled = enabled;
}

IComponent* IComponent::Clone() const
{
	IComponent* clone = DoClone();
	assert(typeid(*clone) == typeid(*this));

	return clone;
}

void IComponent::ReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter(std::string("is_enabled"), &mIsEnabled);
	DoReadFrom(source);
}

void IComponent::WriteTo(core::utils::ISerializer* destination)
{
	destination->BeginAttribute(std::string("component"));
	destination->WriteParameter(std::string("is_enabled"), mIsEnabled);
	DoWriteTo(destination);
	destination->EndAttribute();
}
