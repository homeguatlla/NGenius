#include "stdafx.h"
#include "TexturesLibrary.h"

#include "Texture.h"
#include "TextureArray.h"
#include "TextureCubemap.h"
#include "../../utils/serializer/IDeserializer.h"
#include "../../utils/Log.h"
#include "../Memory.h"

#include <GL/glew.h>

#include <algorithm>

TexturesLibrary::TexturesLibrary(const glm::vec2& screenSize) :
	mScreenSize(screenSize),
	mTextureCubemapUnit(0)
{
}

TexturesLibrary::~TexturesLibrary()
{

}

void TexturesLibrary::Load()
{
	CreateDepthTexture("depth_texture", mScreenSize);
	//LoadTexture("stone", "data/models/stone/diffuse.png", true, false);

	//LoadTexture("hud_map", "data/ui/hud/hud_map.png", false, false);

	//CreateColorTexture("map", glm::vec2(256, 256));

	//LoadTexture("grass2", "data/textures/grass2.png", false, false);
	//LoadTexture("grass3", "data/textures/grass3.png", false, false);
	//LoadTexture("grass5", "data/textures/grass5.png", false, false);

	//LoadTexture("barrel_diffuse", "data/models/barrel/barrel.png", false, false);
	//LoadTexture("barrel_normalmap", "data/models/barrel/barrelNormal.png", false, false);

	LoadTexturesPendingToLoad();
}

void TexturesLibrary::LoadTexture(std::string name, std::string filename, bool hasMipmapping, bool hasWrapping)
{
	Texture* texture = DBG_NEW Texture();
	texture->Load(filename, hasMipmapping, hasWrapping);
	AddOrReplaceElement(name, texture);
}

ITexture* TexturesLibrary::CreateColorTexture(std::string name, const glm::vec2& size)
{
	Texture* texture = DBG_NEW Texture();
	texture->CreateTexture(static_cast<int>(size.x), static_cast<int>(size.y));
	AddOrReplaceElement(name, texture);

	return texture;
}

void TexturesLibrary::CreateDepthTexture(std::string name, unsigned int width, unsigned int height)
{
	Texture* texture = DBG_NEW Texture();
	texture->CreateDepthTexture(width, height);
	AddOrReplaceElement(name, texture);
}

ITexture* TexturesLibrary::CreateDepthTexture(const std::string& name, const glm::ivec2& size)
{
	Texture* texture = DBG_NEW Texture();
	texture->SetUnit(GL_TEXTURE25);
	texture->CreateDepthTexture(size.x, size.y);
	AddOrReplaceElement(name, texture);

	return texture;
}

void TexturesLibrary::LoadTexturesPendingToLoad()
{
	for (TextureTuple tuple : mTexturesPendingToLoad)
	{
		//TODO hay que ver como mejorar esto si hace falta.
		//ahora mismo, tres texturas con distinto name pueden hacer referencia al mismo archivo. No se cargará más que una.
		//suponiendo que las que se cargan directamente sin ser añadidas a mTexturesPendingToLoad se cargasen mediante esta lista.
		
		Texture* texture = DBG_NEW Texture();
		bool loaded = texture->Load(std::get<1>(tuple), true, true);
		if (loaded)
		{
			AddOrReplaceElement(std::get<0>(tuple), texture);
			if (std::get<2>(tuple) != nullptr)
			{
				std::get<2>(tuple)(std::get<0>(tuple), texture);
			}
		}
		else
		{
			delete texture;
		}
	}
	mTexturesPendingToLoad.clear();
}

void TexturesLibrary::ReadFrom(core::utils::IDeserializer * source)
{
	source->BeginAttribute(std::string("textures_library"));
	unsigned int numElements = source->ReadNumberOfElements();

	source->BeginAttribute();
	do
	{
		std::string nodeName = source->GetCurrentNodeName();

		if (nodeName == "texture")
		{
			ReadTextureFrom(source);
		}
		else if(nodeName == "texture_array")
		{
			ReadTextureArrayFrom(source);
		}
		else if (nodeName == "texture_cubemap")
		{
			ReadTextureCubemapFrom(source);
		}
		source->NextAttribute();
		numElements--;

	} while (numElements > 0);

	source->EndAttribute();
	source->EndAttribute();
}

void TexturesLibrary::ReadTextureFrom(core::utils::IDeserializer * source)
{
	std::string textureName;
	source->ReadParameter("name", textureName);

	std::string filename;
	source->ReadParameter("filename", filename);
	bool hasMippaming = false;
	bool hasWrapping = false;
	source->ReadParameter("has_mippaing", &hasMippaming);
	source->ReadParameter("has_wrapping", &hasWrapping);

	LoadTexture(textureName, filename, hasMippaming, hasWrapping);
}

void TexturesLibrary::ReadFilenamesFrom(core::utils::IDeserializer * source, std::vector<std::string>& filenames)
{
	unsigned int numFilenames = source->ReadNumberOfElements();
	source->BeginAttribute(std::string("texture"));

	for (unsigned int i = 0; i < numFilenames; ++i)
	{
		std::string filename;
		bool found = source->ReadParameter("filename", filename);
		if (found)
		{
			filenames.push_back(filename);
		}
		source->NextAttribute();
	}
}

void TexturesLibrary::ReadTextureCubemapFrom(core::utils::IDeserializer* source)
{
	std::string textureName;
	source->ReadParameter("name", textureName);
	std::vector<std::string> filenames;
	ReadFilenamesFrom(source, filenames);

	if (!filenames.empty())
	{
		TextureCubemap* cubemap = DBG_NEW TextureCubemap();
		cubemap->Load(filenames, GL_TEXTURE9 + mTextureCubemapUnit);
		mTextureCubemapUnit++;
		AddOrReplaceElement(textureName, cubemap);
	}
	else
	{
		Log(Log::LOG_WARNING) << "Texture array " << textureName << " empty!";
	}
	source->EndAttribute();
}

void TexturesLibrary::ReadTextureArrayFrom(core::utils::IDeserializer * source)
{
	std::string textureName;
	source->ReadParameter("name", textureName);
	std::vector<std::string> filenames;
	ReadFilenamesFrom(source, filenames);
	
	if (!filenames.empty())
	{
		TextureArray* textureArray = DBG_NEW TextureArray();
		textureArray->Load(filenames, true);
		AddOrReplaceElement(textureName, textureArray);
	}
	else
	{
		Log(Log::LOG_WARNING) << "Texture array " << textureName << " empty!";
	}
	source->EndAttribute();
}

void TexturesLibrary::WriteTo(core::utils::ISerializer * destination)
{
}

void TexturesLibrary::AddTextureNameToLoad(const std::string& name, const std::string& filename, std::function<void(const std::string& textureName, ITexture* texture)> callback)
{
	bool found = std::find_if(mTexturesPendingToLoad.begin(), mTexturesPendingToLoad.end(), [&](const TextureTuple& tuple) { 
		return std::get<0>(tuple).compare(name) == 0; }) != mTexturesPendingToLoad.end();
	if (!HasElement(name) && !found)
	{
		mTexturesPendingToLoad.push_back(std::make_tuple(name, filename, callback));
	}
}