#include "stdafx.h"
#include "TexturesLibrary.h"

#include "ITexture.h"
#include "Texture.h"
#include "TextureArray.h"
#include "TextureCubemap.h"
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
	Texture* texture = new Texture();
	texture->Load("data/models/stone/diffuse.png", mCurrentTextureUnit++, true, false);
	AddElement("stone", texture);

	texture = new Texture();
	texture->Load("data/textures/terrain_heightmap_1024.png", ++mCurrentTextureUnit, false, false);
	AddElement("terrain_heightmap", texture);

	texture = new Texture();
	texture->Load("data/textures/terrain_blendmap_1024.png", ++mCurrentTextureUnit, false, false);
	AddElement("terrain_blendmap", texture);

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
	AddElement("cubemap", cubemap);

	texture = new Texture();
	texture->Load("data/ui/hud/hud_map.png", ++mCurrentTextureUnit, false, false);
	AddElement("hud_map", texture);
	
	texture = new Texture();
	texture->CreateTexture(++mCurrentTextureUnit, 256, 256);
	AddElement("map", texture);

	texture = new Texture();
	texture->CreateTexture(++mCurrentTextureUnit, 320 * 2, 240 * 2);
	AddElement("reflection_water", texture);

	texture = new Texture();
	texture->CreateTexture(++mCurrentTextureUnit, 320 * 2, 240 * 2);
	AddElement("refraction_water", texture);

	texture = new Texture();
	texture->Load("data/textures/waterDUDV.png", ++mCurrentTextureUnit, false, true);
	AddElement("distorsion_water", texture);

	texture = new Texture();
	texture->Load("data/textures/normal_water.png", ++mCurrentTextureUnit, false, true);
	AddElement("normal_water", texture);

	texture = new Texture();
	texture->CreateDepthTexture(++mCurrentTextureUnit, 320 * 2, 240 * 2);
	AddElement("refraction_depth_water", texture);	

	texture = new Texture();
	texture->Load("data/textures/smoke_64.png", ++mCurrentTextureUnit, false, false);
	AddElement("smoke", texture);

	texture = new Texture();
	texture->Load("data/textures/grid_64.png", ++mCurrentTextureUnit, false, true);
	AddElement("yellow_grid", texture);

	texture = new Texture();
	texture->Load("data/models/stall/stall.png", ++mCurrentTextureUnit, false, false);
	AddElement("stall", texture);

	LoadTexturesPendingToLoad();
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
		texture->Load(std::get<1>(tuple), ++mCurrentTextureUnit, true, true);
		AddElement(std::get<0>(tuple), texture);
		if (std::get<2>(tuple) != nullptr)
		{
			std::get<2>(tuple)(std::get<0>(tuple), texture);
		}
	}
	mTexturesPendingToLoad.clear();
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
