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

	bool Load(const std::string& filename, bool hasMipmapping, bool hasWrapping);
	void CreateTexture(int width, int height);
	void CreateDepthTexture(int width, int height);
	glm::vec4 GetColor(glm::vec2 uv) const;
};

