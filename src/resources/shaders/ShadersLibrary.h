#pragma once
#include <map>
#include "../BaseLibrary.h"
#include "../../utils/serializer/ISerializable.h"


class IShaderProgram;

class ShadersLibrary : public core::utils::ISerializable, public BaseLibrary<IShaderProgram*>
{
public:
	ShadersLibrary();
	virtual ~ShadersLibrary() = default;

	void Load();

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;

private:
	void AddShader(const char* name, IShaderProgram* shader);
};

