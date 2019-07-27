#pragma once
#include "Transformation.h"

#include "renderers/IRenderer.h"
#include "renderers/VerticesRenderer.h"

#include "materials/IMaterial.h"
#include "models/Model.h"
#include "systems/renderSystem/RenderSystem.h"

#include "../utils/serializer/ISerializable.h"
#include "../utils/serializer/XMLSerializer.h"
#include "../utils/serializer/IDeserializer.h"
#include "../Memory.h"
#include "IGameEntity.h"
#include "InstantiableObject.h"
#include "../utils/Log.h"

#include<map>
#include<typeinfo>
#include<string>


template<class TD>
class BaseGameEntity : public IGameEntity
{
public:
	BaseGameEntity() = default;
	explicit BaseGameEntity(Transformation* transformation, IRenderer* renderer);
	explicit BaseGameEntity(Transformation* transformation);
	~BaseGameEntity();

	// Heredado vía IGameEntity
	void Init(RenderSystem* renderSystem) override;
	int GetID() const override { return mID; }
	bool IsEnabled() const override	{ return mIsEnabled; }

	Transformation* GetTransformation() override { return mTransformation; }
	const Transformation* GetTransformation() const override { return mTransformation; }

	IRenderer* GetRenderer() const override { return mRenderer;	}
	void SetRenderer(IRenderer* renderer) override;

	virtual void Update(float elapsedTime) override;
	virtual void Build(RenderSystem* renderSystem) override;

	virtual IGameEntity* DoClone() const override;
	IGameEntity* Clone();

	static std::string GetClassName() { return std::string("GameEntity"); }
	static IGameEntity* Create()
	{
		//TD& derived = static_cast<TD&>(*this);
		return TD::DoCreate();
	}

	static IGameEntity* DoCreate() { return DBG_NEW BaseGameEntity<TD>(); }

	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;

protected:
	Transformation* mTransformation;
	IRenderer* mRenderer;
	bool mIsEnabled;
	int mID;
	
	static unsigned int IDCounter;

	//To load an entity
	std::string mModelName;
	std::string mMaterialName;
	std::string mRendererName;
	
	int mRendererLayer;
};

template<class TD>
unsigned BaseGameEntity<TD>::IDCounter = 0;

template<class TD>
BaseGameEntity<TD>::BaseGameEntity(Transformation* transformation, IRenderer* renderer) :
	mTransformation(transformation),
	mRenderer(renderer),
	mIsEnabled(true)
{
	if (renderer != nullptr)
	{
		renderer->SetParent(this);
	}
	mID = ++IDCounter;

}

template<class TD>
BaseGameEntity<TD>::BaseGameEntity(Transformation* transformation) : BaseGameEntity(transformation, nullptr)
{
}

template<class TD>
BaseGameEntity<TD>::~BaseGameEntity()
{
	for (auto& pair : mComponents)
	{
		delete pair.second;
	}
	mComponents.clear();
	if (mTransformation != nullptr)
	{
		delete mTransformation;
		mTransformation = nullptr;
	}
	if (mRenderer != nullptr)
	{
		delete mRenderer;
		mRenderer = nullptr;
	}
}

template<class TD>
void BaseGameEntity<TD>::Init(RenderSystem* renderSystem)
{
	for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		it->second->Init(renderSystem);
	}
}

template<class TD>
void BaseGameEntity<TD>::SetRenderer(IRenderer* renderer)
{
	mRenderer = renderer;
	if (renderer != nullptr)
	{
		renderer->SetParent(this);
	}
}

template<class TD>
void BaseGameEntity<TD>::Update(float elapsedTime)
{
	if (mIsEnabled)
	{
		for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			it->second->Update(elapsedTime);
		}
	}
}

template<class TD>
void BaseGameEntity<TD>::Build(RenderSystem* renderSystem)
{
	Model* model = renderSystem->GetModel(mModelName);
	IMaterial* material = renderSystem->GetMaterial(mMaterialName);
	if (model != nullptr && material != nullptr)
	{
		IRenderer* renderer = nullptr;
		if (!mRendererName.empty())
		{
			renderer = InstantiableObject::CreateRenderer(mRendererName, model, material);
		}
		else
		{
			//By default
			renderer = DBG_NEW  VerticesRenderer(model, material);
		}
		if (renderer != nullptr)
		{
			renderer->SetLayer(mRendererLayer);
			SetRenderer(renderer);
		}
		else
		{
			Log(Log::LOG_ERROR) << "Error creating " << mRendererName << " renderer for model " << mModelName << "\n";
		}
	}
}

template<class TD>
IGameEntity* BaseGameEntity<TD>::DoClone() const
{
	IGameEntity* clone = DBG_NEW  BaseGameEntity(new Transformation(*GetTransformation()));
	if (GetRenderer() != nullptr)
	{
		clone->SetRenderer(GetRenderer()->Clone());
	}

	return clone;
}

template<class TD>
IGameEntity* BaseGameEntity<TD>::Clone()
{
	IGameEntity* clone = DoClone();

	assert(typeid(*clone) == typeid(*this));

	for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		clone->AddComponent(it->first, it->second->Clone());
	}

	return clone;
}

template<class TD>
void BaseGameEntity<TD>::ReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter("model", mModelName);
	source->ReadParameter("material", mMaterialName);
	source->ReadParameter("renderer", mRendererName);
	source->ReadParameter("is_enabled", &mIsEnabled);
	source->ReadParameter("layer", &mRendererLayer);
	mTransformation = DBG_NEW  Transformation();
	mTransformation->ReadFrom(source);
}

template<class TD>
void BaseGameEntity<TD>::WriteTo(core::utils::ISerializer* destination)
{
	destination->BeginAttribute(std::string("entity"));
	destination->WriteParameter(std::string("id"), mID, "");
	destination->WriteParameter(std::string("is_enabled"), mIsEnabled);
	if (GetRenderer() != nullptr)
	{
		IRenderer* renderer = GetRenderer();
		unsigned int modelID = renderer->GetModel()->GetID();
		unsigned int materialID = renderer->GetMaterial()->GetMaterialID();
		destination->WriteParameter(std::string("model"), modelID);
		destination->WriteParameter(std::string("material"), materialID);
	}
	mTransformation->WriteTo(destination);

	destination->BeginAttribute(std::string("components"));
	for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		it->second->WriteTo(destination);
	}
	destination->EndAttribute();
	destination->EndAttribute();
}