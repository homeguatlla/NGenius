#pragma once

#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <glm/glm.hpp>
#include "../BaseLibrary.h"
#include "ITexture.h"
#include "../../utils/serializer/ISerializable.h"

class TexturesLibrary : public core::utils::ISerializable, public BaseLibrary<ITexture*>
{
	typedef std::tuple<std::string, std::string, std::function<void(const std::string& fontName, ITexture* texture)>> TextureTuple;
	std::vector<TextureTuple> mTexturesPendingToLoad;
	int mCurrentTextureUnit;
	const glm::vec2 mScreenSize;

public:
	TexturesLibrary(const glm::vec2& screenSize);
	virtual ~TexturesLibrary();

	void Load();
	void AddTextureNameToLoad(const std::string& name, const std::string& filename, std::function<void(const std::string& textureName, ITexture* texture)> callback = nullptr);
	ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);
	ITexture* CreateColorTexture(std::string name, const glm::vec2& size);
	void LoadTexturesPendingToLoad();

	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer * source) override;
	virtual void WriteTo(core::utils::ISerializer * destination) override;

private:
	void LoadTexture(std::string name, std::string filename, bool hasMipmapping = false, bool hasWrapping = false);
	void CreateDepthTexture(std::string name, unsigned int width, unsigned int height);
	

	void ReadTextureFrom(core::utils::IDeserializer * source);
	void ReadTextureArrayFrom(core::utils::IDeserializer * source);
	void ReadTextureCubemapFrom(core::utils::IDeserializer* source);

	void ReadFilenamesFrom(core::utils::IDeserializer * source, std::vector<std::string>& filenames);
};

