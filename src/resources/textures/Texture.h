#pragma once
#include <string>
#include <glm/glm.hpp>
#include "PNGLoader.h"
#include "ITexture.h"

class Texture : public ITexture
{
	PNGLoader mLoader;

public:
	Texture();
	~Texture();

	void Load(const std::string& filename, unsigned int textureUnit, bool hasMipmapping, bool hasWrapping);
	void CreateTexture(int textureUnit, int width, int height);
	void CreateDepthTexture(int textureUnit, int width, int height);
	glm::vec4 GetColor(glm::vec2 uv) const;
};

