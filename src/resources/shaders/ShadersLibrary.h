#pragma once
#include <map>
#include "../BaseLibrary.h"
#include "../../utils/serializer/ISerializable.h"


class IShaderProgram;

class ShadersLibrary : public core::utils::ISerializable, public BaseLibrary<IShaderProgram*>
{
public:
	static const char* SHADOW_SHADER_NAME;
	static const char* DEFAULT_SHADER_NAME;
	static const char* OVERDRAW_SHADER_NAME;
	static const char* TEXT_SHADER_NAME;
	static const char* GUI_SHADER_NAME;
	static const char* NORMAL_MAP_SHADER_NAME;
	static const char* CONTRAST_SHADER_NAME;

	ShadersLibrary();
	virtual ~ShadersLibrary() = default;

	void Load();

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;

private:
	void AddShader(const char* name, IShaderProgram* shader);
};

