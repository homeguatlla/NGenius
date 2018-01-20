#include "stdafx.h"
#include "TextureGenerator.h"
#include "PNGLoader.h"

#include <vector>

using namespace glm;

const int BPP = 4;

TextureGenerator::TextureGenerator()
{
}


TextureGenerator::~TextureGenerator()
{
}


void TextureGenerator::Generate(std::string filename, int size, float heightScale, bool generateNormals)
{
	std::vector<unsigned char> heightmap(BPP * size * size, 255);
	GenerateHeighmap(size, heightmap, size);

	if (generateNormals)
	{
		GenerateNormals(size, heightmap, size, heightScale);
	}

	SavePNG(filename, size, heightmap);
}
void TextureGenerator::GenerateOnlyBlendmap(std::string filename, int size)
{
	std::vector<unsigned char> heightmap(BPP * size * size, 255);
	GenerateHeighmap(size, heightmap, size);

	float minHeight = 99999.0;
	float maxHeight = -99999.0;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int d = i * size + j;
			float height = heightmap[BPP * d];
			if (height > maxHeight)
			{
				maxHeight = height;
			}
			if (height < minHeight)
			{
				minHeight = height;
			}
		}
	}

	std::vector<unsigned char> blendmap(BPP * size * size, 255);
	GenerateBlendmap(size, heightmap, blendmap, size, minHeight, maxHeight);
	SavePNG(filename, size, blendmap);
}

void TextureGenerator::SavePNG(std::string& filename, int size, std::vector<unsigned char>& data)
{
	//save png
	PNGLoader pngLoader;

	pngLoader.SetHeight(size);
	pngLoader.SetWidth(size);
	pngLoader.SetBitDepth(8);
	pngLoader.SetColorType(PNG_COLOR_TYPE_RGB_ALPHA);
	pngLoader.SetData(&data[0]);

	pngLoader.WritePNGFile(filename.c_str());
}

float TextureGenerator::CalculateHeight(NoiseGenerator& ng, glm::vec2 p)
{
	/*vec2 q = vec2(ng.FBM(p + vec2(0.0f, 0.0f)),
		ng.FBM(p + vec2(5.2f, 1.3f)));
	vec2 r = vec2(ng.FBM(p + 4.0f * q + vec2(1.7f, 9.2f)),
		ng.FBM(p + 4.0f * q + vec2(8.3f, 2.8f)));

	return ng.FBM(p + 4.0f * q);
	*/
	return ng.FBM(p);
}

void TextureGenerator::GenerateHeighmap(int size, std::vector<unsigned char>& heightmap, int length)
{
	NoiseGenerator ng;

	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size; j++)
		{
			glm::vec2 st(i, j);
			st /= size;

			float height = max(min(CalculateHeight(ng, st * 3.0f), 1.0f), 0.0f);
			int d = i * size + j;
			heightmap[BPP * d] = heightmap[BPP * d + 1] = heightmap[BPP * d + 2] = heightmap[BPP * d + 3] = static_cast<int>(height * 255);
			//imageheight[BPP * d + 3] = 255;
			//(height - minHeight) / (maxHeight - minHeight) * 255
		}
	}

	float radius = size/3.0f;
	float min = 0.0f;
	ModifyToIsland(heightmap, size, radius, min);
}

void TextureGenerator::ModifyToIsland(std::vector<unsigned char>& heightmap, int size, float radius, float min)
{
	glm::vec2 center(size / 2, size / 2);
	//center /= size;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			glm::vec2 st(i, j);
			int d = i * size + j;
			float distanceToCenter = glm::distance(st, center);
			glm::vec2 point = center + glm::normalize(st - center) * radius;
			int dMax = (int)point.x * size + (int)point.y;
			float maxHeight = heightmap[BPP * dMax] / 255.0f;
			float newHeight = heightmap[BPP * d] / 255.0f;

			if ( distanceToCenter > radius && distanceToCenter < radius + 40.0f)
			{					
				newHeight = maxHeight - maxHeight *glm::clamp((distanceToCenter - radius) / (size / 2.0f - radius), 0.0f, 1.0f);
			}
			else if (distanceToCenter > radius)
			{
				maxHeight = maxHeight - maxHeight *glm::clamp((distanceToCenter - radius + 40.0f) / (size / 2.0f - radius + 40.0f), 0.0f, 1.0f);
				newHeight = maxHeight - maxHeight/5 *glm::clamp((distanceToCenter - radius) / (size / 10.0f - radius), 0.0f, 1.0f);
			}
			heightmap[BPP * d] = heightmap[BPP * d + 1] = heightmap[BPP * d + 2] = heightmap[BPP * d + 3] = static_cast<int>(newHeight * 255);
		}
	}
}

glm::vec4 TextureGenerator::CalculateBlendColor(unsigned int height, float minHeight, float maxHeight)
{
	unsigned int portion = static_cast<unsigned int>((maxHeight - minHeight ) / 5);
	if (height < portion * 3.5)
	{
		return glm::vec4(255.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (height >= portion * 3.5 && height < portion * 4.0)
	{
		return glm::vec4(0.0f, 255.0f, 0.0f, 0.0f);
	}
	else if (height >= portion * 4.0 && height < portion * 4.5)
	{
		return glm::vec4(0.0f, 0.0f, 255.0f, 0.0f);
	}
	else 
	{
		return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

void TextureGenerator::GenerateBlendmap(int size, std::vector<unsigned char>& heightmap, std::vector<unsigned char>& blendmap, int length, float minHeight, float maxHeight)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int d = i * size + j;
			unsigned int height = heightmap[BPP * d];
			vec4 color = CalculateBlendColor(height, minHeight, maxHeight);
			blendmap[BPP * d] = static_cast<unsigned int>(color.x);
			blendmap[BPP * d + 1] = static_cast<unsigned int>(color.y);
			blendmap[BPP * d + 2] = static_cast<unsigned int>(color.z);
			blendmap[BPP * d + 3] = 255;// static_cast<unsigned int>(color.t);
		}
	}
}


void TextureGenerator::GenerateNormals(int size, std::vector<unsigned char>& heightmap, int length, float heightScale)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vec3 normal = CalculateNormal(i, j, heightmap, length, heightScale);
			normal = (normal + vec3(1.0f, 1.0f, 1.0f)) * 0.5f;
			normal *= 255.0f;
			int d = i * size + j;
			heightmap[BPP * d] = static_cast<unsigned int>(normal.x);
			heightmap[BPP * d + 1] = static_cast<unsigned int>(normal.y);
			heightmap[BPP * d + 2] = static_cast<unsigned int>(normal.z);
		}
	}
}

glm::vec3 TextureGenerator::CalculateNormal(int i, int j, std::vector<unsigned char>& heightmap, int length, float heightScale)
{
	int muevei[] = { 1, 0, -1, 0 };
	int muevej[] = { 0, 1, 0, -1 };

	float scale = heightScale / 255.0f;
	vec3 p0(i, heightmap[(i + j * length) * 4 + 3] * scale, j);
	
	vec3 nor(0, 0, 0);
	for (int k = 0; k<4; k++) 
	{
		int nexti1 = i + muevei[k];
		int nextj1 = j + muevej[k];
		int nexti2 = i + muevei[(k + 1) % 4];
		int nextj2 = j + muevej[(k + 1) % 4];
		if (IsInterior(nexti1, nextj1, length) && IsInterior(nexti2, nextj2, length))
		{
			int offset1 = (nexti1 + nextj1 * length) * 4 + 3;
			int offset2 = (nexti2 + nextj2 * length) * 4 + 3;
			vec3 p1(nexti1, heightmap[offset1] * scale, nextj1);
			vec3 p2(nexti2, heightmap[offset2] * scale, nextj2);
			nor += cross(p2 - p0, p1 - p0);
		}
	}
	return normalize(nor);
}

glm::vec3 TextureGenerator::CalculateNormalByTriangles(int i, int j, std::vector<unsigned char>& heightmap, int length, float heightScale)
{
	float scale = heightScale / 255.0f;
	vec3 p[9];

	p[8] = glm::vec3(i, heightmap[(i + j * length) * 4 + 3] * scale, j);
	for (int k = 0; k < 8; k++)
	{
		p[k] = p[8];
	}

	if (IsInterior(i, j - 1, length))
	{
		int x = i;
		int z = j - 1;
		p[0] = glm::vec3(x, heightmap[(x + z * length) * 4 + 3] * scale, z);
	}
	if (IsInterior(i + 1, j - 1, length))
	{
		int x = i + 1;
		int z = j - 1;
		p[1] = glm::vec3(x, heightmap[(x + z * length) * 4 + 3] * scale, z);
	}
	if (IsInterior(i + 1, j, length))
	{
		int x = i + 1;
		int z = j;
		p[2] = glm::vec3(x, heightmap[(x + z * length) * 4 + 3] * scale, z);
	}
	if (IsInterior(i + 1, j + 1, length))
	{
		int x = i + 1;
		int z = j + 1;
		p[3] = glm::vec3(x, heightmap[(x + z * length) * 4 + 3] * scale, z);
	}
	if (IsInterior(i, j + 1, length))
	{
		int x = i;
		int z = j + 1;
		p[4] = glm::vec3(x, heightmap[(x + z * length) * 4 + 3] * scale, z);
	}
	if (IsInterior(i - 1, j + 1, length))
	{
		int x = i - 1;
		int z = j + 1;
		p[5] = glm::vec3(x, heightmap[(x + z * length) * 4 + 3] * scale, z);
	}
	if (IsInterior(i - 1, j, length))
	{
		int x = i - 1;
		int z = j;
		p[6] = glm::vec3(x, heightmap[(x + z * length) * 4 + 3] * scale, z);
	}
	if (IsInterior(i - 1, j - 1, length))
	{
		int x = i - 1;
		int z = j - 1;
		p[7] = glm::vec3(x, heightmap[(x + z * length) * 4 + 3] * scale, z);
	}

	glm::vec3 normal(0);

	for (int k = 0; k < 8; ++k)
	{
		if (p[k] != p[8] && p[(k+1)%8] != p[8])
		{
			glm::vec3 v1 = p[k] - p[8];
			glm::vec3 v2 = p[(k + 1) % 8] - p[8];

			normal += cross(v2, v1);
		}
	}

	return normalize(normal);
}

bool TextureGenerator::IsInterior(int i, int j, int size)
{
	return 0 <= i && i<size && 0 <= j && j<size;
}