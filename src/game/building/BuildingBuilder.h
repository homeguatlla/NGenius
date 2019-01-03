#pragma once
#include <glm/glm.hpp>
#include <vector>

class GameScene;

class BuildingBuilder
{
public:
	typedef struct BuildWall
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		std::string textureName;
		std::string normalTextureName;
		float textureTile;

		BuildWall(glm::vec3& p, std::string& texture, std::string& normal, float tile) : position(p), rotation(0.0f), scale(1.0f), textureName(texture), normalTextureName(normal), textureTile(tile) {}
		BuildWall(glm::vec3& p, glm::vec3& rotation, std::string& texture, std::string& normal, float tile) : position(p), rotation(rotation), scale(1.0f), textureName(texture), normalTextureName(normal), textureTile(tile) {}
		BuildWall(glm::vec3& p, glm::vec3& rotation, glm::vec3& scale, std::string& texture, std::string& normal, float tile) : position(p), rotation(rotation), scale(scale), textureName(texture), normalTextureName(normal), textureTile(tile) {}
	} BuildWall;

private:
	std::vector<std::vector<unsigned int>> mWallsMatrix;
	std::vector<BuildWall> mWalls;
	glm::vec3 mSize;
	GameScene* mScene;
	glm::vec3 mCentre;

public:
	BuildingBuilder(GameScene* scene);
	~BuildingBuilder();

	void SetWallsMatrix(std::vector<std::vector<unsigned int>>& mWallsMatrix);
	void SetCentre(glm::vec3& centre);

	void Build();

private:
	void CreateWall(BuildWall& wallData);
	std::vector<BuildWall> CreateBuildingData(unsigned int piece, float x, float z);
};

