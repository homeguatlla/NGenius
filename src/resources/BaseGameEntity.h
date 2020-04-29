#pragma once
#include "Transformation.h"

#include "src/NGenius.h"
#include "renderers/IRenderer.h"
#include "renderers/VerticesRenderer.h"

#include "materials/IMaterial.h"
#include "models/Model.h"
#include "systems/renderSystem/RenderSystem.h"
#include "components/LODComponent.h"

#include "src/utils/serializer/ISerializable.h"
#include "src/utils/serializer/XMLSerializer.h"
#include "src/utils/serializer/IDeserializer.h"
#include "src/utils/Log.h"
#include "Memory.h"
#include "IGameEntity.h"
#include "InstantiableObject.h"


#include<map>
#include<typeinfo>
#include<string>

class NGenius;

template<class TD>
class BaseGameEntity : public IGameEntity
{
public:
	BaseGameEntity();
	BaseGameEntity(Transformation* transformation, IRenderer* renderer);
	BaseGameEntity(Transformation* transformation);
	~BaseGameEntity();

	// Heredado vía IGameEntity
	void Init(GameScene* scene, RenderSystem* renderSystem) final;
	
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem);

	int GetID() const override { return mID; }
	bool IsEnabled() const override	{ return mIsEnabled; }
	void SetEnabled(bool enable) override { mIsEnabled = enable; }

	void SetShouldBeCreatedOnGround(bool onGround) { mShouldBeCreatedOnGround = onGround; }
	bool ShouldBeCreatedOnGround() const override { return mShouldBeCreatedOnGround; }

	void SetName(const std::string& name) { mName = name; }
	std::string GetName() const { return mName; }

	Transformation* GetTransformation() override { return mTransformation; }
	const Transformation* GetTransformation() const override { return mTransformation; }

	IRenderer* GetRenderer() const override { return mRenderer;	}
	void SetRenderer(IRenderer* renderer) override;

	virtual void Update(float elapsedTime) override;
	virtual void Build(NGenius* engine) override;

	virtual std::shared_ptr<IGameEntity> DoClone() const override;
	std::shared_ptr<IGameEntity> Clone();

	static std::string GetClassName() { return std::string("GameEntity"); }
	static std::shared_ptr<IGameEntity> Create()
	{
		//TD& derived = static_cast<TD&>(*this);
		return TD::DoCreate();
	}

	static std::shared_ptr<IGameEntity> DoCreate() { return std::make_shared<BaseGameEntity<TD>>(); }

	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;

protected:
	Transformation* mTransformation;
	IRenderer* mRenderer;
	bool mIsEnabled;
	int mID;
	std::string mName;
	
	static unsigned int IDCounter;

	//To load an entity
	std::string mModelName;
	std::string mMaterialName;
	std::string mRendererName;
	
	bool mShouldBeCreatedOnGround;
	int mRendererLayer;
};

template<class TD>
unsigned BaseGameEntity<TD>::IDCounter = 0;

template<class TD>
inline BaseGameEntity<TD>::BaseGameEntity() :
	mTransformation(nullptr),
	mRenderer(nullptr),
	mIsEnabled(true),
	mModelName(""),
	mMaterialName(""),
	mRendererName(""),
	mShouldBeCreatedOnGround(false)
{
}

template<class TD>
BaseGameEntity<TD>::BaseGameEntity(Transformation* transformation, IRenderer* renderer) :
	mTransformation(transformation),
	mRenderer(renderer),
	mIsEnabled(true),
	mModelName(""),
	mMaterialName(""),
	mRendererName(""),
	mShouldBeCreatedOnGround(false)
{
	mID = ++IDCounter;
	mName = mID;
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
void BaseGameEntity<TD>::Init(GameScene* scene, RenderSystem* renderSystem)
{
	if (mRenderer != nullptr)
	{
		mRenderer->SetParent(shared_from_this());
	}

	this->DoInit(scene, renderSystem);

	//if has LOD we must init first in order to assign a renderer to the entity and
	//the other components work properly
	if (HasComponent<LODComponent>())
	{
		GetComponent<LODComponent>()->Init(scene, renderSystem);
	}

	//Warning LODComponent will be initialized twice.
	for (IComponentsIterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		it->second->Init(scene, renderSystem);
	}
}

//DOInit will happen just when all entities will have built and are going to pass from newEntitiesToAdd list
//to entitiesList in the scene. In this method, an entity can clone new existing entities due all entities have
//been created and build. The new cloned entities will be added too in the entitiesList in the same frame
template<class TD>
inline void BaseGameEntity<TD>::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}

template<class TD>
void BaseGameEntity<TD>::SetRenderer(IRenderer* renderer)
{
	mRenderer = renderer;
	if (renderer != nullptr)
	{
		renderer->SetParent(shared_from_this());
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

//BUILD method allows the entity to create itself. This way, when an entity will be added from 
//newEntities to add to the list of entities in the scene will be created.
template<class TD>
void BaseGameEntity<TD>::Build(NGenius* engine)
{
	Model* model = engine->GetModel(mModelName);
	IMaterial* material = engine->GetMaterial(mMaterialName);
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
std::shared_ptr<IGameEntity> BaseGameEntity<TD>::DoClone() const
{
	std::shared_ptr<IGameEntity> clone = std::make_shared<BaseGameEntity>(new Transformation(*GetTransformation()));
	if (GetRenderer() != nullptr)
	{
		clone->SetRenderer(GetRenderer()->Clone());
	}

	return clone;
}

template<class TD>
std::shared_ptr<IGameEntity> BaseGameEntity<TD>::Clone()
{
	std::shared_ptr<IGameEntity> clone = DoClone();

	clone->SetName(this->GetName());

	assert(typeid(*clone) == typeid(*shared_from_this()));

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
	source->ReadParameter("name", mName);
	source->ReadParameter("on_ground", &mShouldBeCreatedOnGround);

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