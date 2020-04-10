#pragma once

#include <assert.h>
#include "../BaseLibrary.h"
#include "../../utils/serializer/ISerializable.h"

class IMaterial;
class ITexture;
class IShaderProgram;
class ShadersLibrary;
class TexturesLibrary;


class MaterialsLibrary : public core::utils::ISerializable, public BaseLibrary<IMaterial*>
{
	ShadersLibrary* mShadersLibrary;
	TexturesLibrary* mTexturesLibrary;

public:
	static const char* OVERDRAW_MATERIAL_NAME;
	static const char* WIREFRAME_MATERIAL_NAME;
	static const char* COLLIDER_MATERIAL_NAME;
	static const char* QUADTREE_MATERIAL_NAME;
	static const char* MODEL_MATERIAL_NAME;
	static const char* TEXT_MATERIAL_NAME;
	static const char* TEXT3D_MATERIAL_NAME;
	static const char* GUI_MATERIAL_NAME;

	MaterialsLibrary(TexturesLibrary* texturesLibrary, ShadersLibrary* shadersLibrary);
	virtual ~MaterialsLibrary() = default;

	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	void Load();
	void Build();

	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer * source) override;
	virtual void WriteTo(core::utils::ISerializer * destination) override;

private:
	void ReadMaterialFrom(core::utils::IDeserializer * source);
	void ReadMaterialEffects(IMaterial* material, core::utils::IDeserializer * source);
};

