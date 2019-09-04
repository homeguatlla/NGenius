#include "stdafx.h"
#include "MaterialsLibrary.h"
#include "../shaders/ShadersLibrary.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../InstantiableObject.h"

#include "IMaterial.h"
#include "BasicMaterial.h"
#include "effects/MaterialEffectNormalTexture.h"
#include "effects/MaterialEffectDiffuseTexture.h"
#include "effects/MaterialEffectClippingPlane.h"
#include "effects/MaterialEffectDepthTexture.h"
#include "effects/MaterialEffectDirectionalLightProperties.h"
#include "effects/MaterialEffectFloat.h"
#include "effects/MaterialEffectFloat2.h"
#include "effects/MaterialEffectFloat3.h"
#include "effects/MaterialEffectFloat3Array.h"
#include "effects/MaterialEffectFloat4.h"
#include "effects/MaterialEffectFogProperties.h"
#include "effects/MaterialEffectHeightMapTexture.h"
#include "effects/MaterialEffectMatrix4Array.h"
#include "effects/MaterialEffectParticle.h"
#include "effects/MaterialEffectShadowProperties.h"
#include "effects/MaterialEffectText.h"
#include "effects/MaterialEffectTextureArray.h"
#include "effects/MaterialEffectTextureCubemap.h"
#include "effects/MaterialEffectWater.h"


#include <GL/glew.h>
#include <algorithm>
#include "../../utils/Log.h"
#include "../Memory.h"

const char* MaterialsLibrary::OVERDRAW_MATERIAL_NAME = "overdraw";
const char* MaterialsLibrary::WIREFRAME_MATERIAL_NAME = "wireframe";
const char* MaterialsLibrary::QUADTREE_MATERIAL_NAME = "quadtree";
const char* MaterialsLibrary::MODEL_MATERIAL_NAME = "model";
const char* MaterialsLibrary::TEXT_MATERIAL_NAME = "text";
const char* MaterialsLibrary::TEXT3D_MATERIAL_NAME = "text3d";
const char* MaterialsLibrary::GUI_MATERIAL_NAME = "gui";

MaterialsLibrary::MaterialsLibrary(TexturesLibrary* texturesLibrary, ShadersLibrary* shadersLibrary) :
	mTexturesLibrary(texturesLibrary),
	mShadersLibrary(shadersLibrary)
{
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectNormalTexture>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectDiffuseTexture>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectDirectionalLightProperties>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectClippingPlane>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectDepthTexture>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectFloat>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectFloat2>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectFloat3>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectFloat4>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectFloat3Array>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectFogProperties>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectHeightMapTexture>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectMatrix4Array>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectParticle>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectShadowProperties>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectText>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectTextureArray>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectTextureCubemap>();
	InstantiableObject::RegisterMaterialEffectType<MaterialEffectWater>();
}

IMaterial* MaterialsLibrary::CreateMaterial(const std::string& name, IShaderProgram* shader)
{
	assert(shader != nullptr);

	IMaterial* material = DBG_NEW BasicMaterial(shader);
	AddOrReplaceElement(name, material);

	return material;
}

void MaterialsLibrary::Load()
{
	CreateMaterial(OVERDRAW_MATERIAL_NAME, mShadersLibrary->GetElement("OverdrawShader"));
	CreateMaterial(WIREFRAME_MATERIAL_NAME, mShadersLibrary->GetElement("DefaultShader"));
	CreateMaterial(QUADTREE_MATERIAL_NAME, mShadersLibrary->GetElement("DefaultShader"));
	CreateMaterial(MODEL_MATERIAL_NAME, mShadersLibrary->GetElement("NormalMapShader"));
	CreateMaterial(TEXT_MATERIAL_NAME, mShadersLibrary->GetElement("TextShader"));
	CreateMaterial(TEXT3D_MATERIAL_NAME, mShadersLibrary->GetElement("TextShader"));
	CreateMaterial(GUI_MATERIAL_NAME, mShadersLibrary->GetElement("GUIShader"));
}

void MaterialsLibrary::Build()
{
	BaseLibraryIterator it;

	for (it = mElements.begin(); it != mElements.end(); it++)
	{
		it->second->Build(mTexturesLibrary);
	}
}

void MaterialsLibrary::ReadFrom(core::utils::IDeserializer * source)
{
	source->BeginAttribute(std::string("materials_library"));
	unsigned int numElements = source->ReadNumberOfElements();

	source->BeginAttribute(std::string("material"));
	do
	{
		std::string nodeName = source->GetCurrentNodeName();

	 	if (nodeName == "material")
		{
			ReadMaterialFrom(source);
		}
		
		source->NextAttribute();
		numElements--;

	} while (numElements > 0);

	source->EndAttribute();
	source->EndAttribute();
}

void MaterialsLibrary::WriteTo(core::utils::ISerializer * destination)
{
}

void MaterialsLibrary::ReadMaterialFrom(core::utils::IDeserializer * source)
{
	std::string materialName;
	source->ReadParameter("name", materialName);
	std::string shaderName;
	source->ReadParameter("shader", shaderName);
	IShaderProgram* shader =  mShadersLibrary->GetElement(shaderName);

	IMaterial* material = CreateMaterial(materialName, shader);
	ReadMaterialEffects(material, source);
	
	source->EndAttribute();
}
void MaterialsLibrary::ReadMaterialEffects(IMaterial* material, core::utils::IDeserializer * source)
{
	unsigned int numEffects = source->ReadNumberOfElements();
	source->BeginAttribute(std::string("effect"));

	for (unsigned int i = 0; i < numEffects; ++i)
	{
		std::string effectName;
		bool found = source->ReadParameter("name", effectName);
		if (found)
		{
			IMaterialEffect* materialEffect = InstantiableObject::CreateMaterialEffect(effectName, material);
			if (materialEffect != nullptr)
			{
				materialEffect->ReadFrom(source);
			}
			else
			{
				Log(Log::LOG_WARNING) << "Material effect couldn't be read " << effectName << "\n";
			}
		}
		source->NextAttribute();
	}
}