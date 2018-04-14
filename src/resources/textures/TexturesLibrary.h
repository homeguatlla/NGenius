#pragma once

#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <glm/glm.hpp>
#include "../BaseLibrary.h"


class ITexture;

class TexturesLibrary : public BaseLibrary<ITexture*>
{
	typedef std::tuple<std::string, std::string, std::function<void(const std::string& fontName, ITexture* texture)>> TextureTuple;
	std::vector<TextureTuple> mTexturesPendingToLoad;
	int mCurrentTextureUnit;

public:
	TexturesLibrary();
	virtual ~TexturesLibrary();

	void Load();
	void AddTextureNameToLoad(const std::string& name, const std::string& filename, std::function<void(const std::string& textureName, ITexture* texture)> callback = nullptr);
	ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);
	ITexture* CreateColorTexture(std::string name, const glm::vec2& size);

private:
	void LoadTexture(std::string name, std::string filename, bool hasMipmapping, bool hasWrapping);
	void CreateDepthTexture(std::string name, unsigned int width, unsigned int height);
	void LoadTexturesPendingToLoad();
};

