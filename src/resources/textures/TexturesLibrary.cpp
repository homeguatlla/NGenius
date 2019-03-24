#include "stdafx.h"
#include "TexturesLibrary.h"

#include "Texture.h"
#include "TextureArray.h"
#include "TextureCubemap.h"
#include "../../utils/serializer/IDeserializer.h"
#include "../../utils/Log.h"
#include <GL/glew.h>

#include <algorithm>

TexturesLibrary::TexturesLibrary() : mCurrentTextureUnit(0)
{
}

TexturesLibrary::~TexturesLibrary()
{

}

void TexturesLibrary::Load()
{
	LoadTexture("stone", "data/models/stone/diffuse.png", true, false);
	//LoadTexture("terrain_heightmap", "data/textures/terrain_heightmap_1024.png", false, false);
	//LoadTexture("terrain_blendmap", "data/textures/terrain_blendmap_1024.png", false, false);

	std::vector<std::string> filenames;

	filenames.push_back("data/textures/sand_256.png");
	filenames.push_back("data/textures/grass_256.png");
	filenames.push_back("data/textures/grass_dark_256.png");	
	filenames.push_back("data/textures/rock3.png");
	

	TextureArray* textureArray = new TextureArray();
	textureArray->Load(filenames, ++mCurrentTextureUnit, true);
	AddElement("terrain_array", textureArray);

	filenames.clear();

	filenames.push_back("data/skybox/sky1/right.png");
	filenames.push_back("data/skybox/sky1/left.png");
	filenames.push_back("data/skybox/sky1/top.png");
	filenames.push_back("data/skybox/sky1/bottom.png");
	filenames.push_back("data/skybox/sky1/back.png");
	filenames.push_back("data/skybox/sky1/front.png");

	TextureCubemap* cubemap = new TextureCubemap();
	cubemap->Load(filenames, ++mCurrentTextureUnit);
	AddElement("day_cubemap", cubemap);

	filenames.clear();

	filenames.push_back("data/skybox/sky2/right.png");
	filenames.push_back("data/skybox/sky2/left.png");
	filenames.push_back("data/skybox/sky2/top.png");
	filenames.push_back("data/skybox/sky2/bottom.png");
	filenames.push_back("data/skybox/sky2/back.png");
	filenames.push_back("data/skybox/sky2/front.png");

	cubemap = new TextureCubemap();
	cubemap->Load(filenames, ++mCurrentTextureUnit);
	AddElement("night_cubemap", cubemap);

	LoadTexture("hud_map", "data/ui/hud/hud_map.png", false, false);

	CreateColorTexture("map", glm::vec2(256, 256));

	//LoadTexture("distorsion_water", "data/textures/waterDUDV.png", false, true);
	//LoadTexture("normal_water", "data/textures/normal_water.png", false, true);

	LoadTexture("smoke", "data/textures/smoke_64_.png", false, false);
	LoadTexture("yellow_grid", "data/textures/grid_64.png", false, true);
	LoadTexture("stall", "data/models/stall/stall.png", false, false);
	
	LoadTexture("grass2", "data/textures/grass2.png", false, false);
	LoadTexture("grass3", "data/textures/grass3.png", false, false);
	LoadTexture("grass5", "data/textures/grass5.png", false, false);

	LoadTexture("wind_texture", "data/wind_map.png", false, true);

	LoadTexture("barrel_diffuse", "data/models/barrel/barrel.png", false, false);
	LoadTexture("barrel_normalmap", "data/models/barrel/barrelNormal.png", false, false);

	LoadTexturesPendingToLoad();
}

void TexturesLibrary::LoadTexture(std::string name, std::string filename, bool hasMipmapping, bool hasWrapping)
{
	Texture* texture = new Texture();
	texture->Load(filename, ++mCurrentTextureUnit, hasMipmapping, hasWrapping);
	AddElement(name, texture);
}

ITexture* TexturesLibrary::CreateColorTexture(std::string name, const glm::vec2& size)
{
	Texture* texture = new Texture();
	texture->CreateTexture(++mCurrentTextureUnit, static_cast<int>(size.x), static_cast<int>(size.y));
	AddElement(name, texture);

	return texture;
}

void TexturesLibrary::CreateDepthTexture(std::string name, unsigned int width, unsigned int height)
{
	Texture* texture = new Texture();
	texture->CreateDepthTexture(++mCurrentTextureUnit, width, height);
	AddElement(name, texture);
}

ITexture* TexturesLibrary::CreateDepthTexture(const std::string& name, const glm::ivec2& size)
{
	Texture* texture = new Texture();
	texture->CreateDepthTexture(++mCurrentTextureUnit, size.x, size.y);
	AddElement(name, texture);

	return texture;
}

void TexturesLibrary::LoadTexturesPendingToLoad()
{
	for (TextureTuple tuple : mTexturesPendingToLoad)
	{
		//TODO hay que ver como mejorar esto si hace falta.
		//ahora mismo, tres texturas con distinto name pueden hacer referencia al mismo archivo. No se cargará más que una.
		//suponiendo que las que se cargan directamente sin ser añadidas a mTexturesPendingToLoad se cargasen mediante esta lista.
		
		Texture* texture = new Texture();
		bool loaded = texture->Load(std::get<1>(tuple), ++mCurrentTextureUnit, true, true);
		if (loaded)
		{
			AddElement(std::get<0>(tuple), texture);
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

	source->BeginAttribute(std::string("texture"));
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

void TexturesLibrary::ReadTextureArrayFrom(core::utils::IDeserializer * source)
{
	std::string textureName;
	source->ReadParameter("name", textureName);

	unsigned int numTextures = source->ReadNumberOfElements();
	source->BeginAttribute(std::string("texture"));

	std::vector<std::string> filenames;
	for (int i = 0; i < numTextures; ++i)
	{
		std::string filename;
		bool found = source->ReadParameter("filename", filename);
		if (found)
		{
			filenames.push_back(filename);
		}
		source->NextAttribute();
	}
	if (!filenames.empty())
	{
		TextureArray* textureArray = new TextureArray();
		textureArray->Load(filenames, ++mCurrentTextureUnit, true);
		AddElement(textureName, textureArray);
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
