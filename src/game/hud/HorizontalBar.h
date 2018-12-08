#pragma once

#include <glm/glm.hpp>
#include <string>
class GameScene;
class GameEntity;
class Text;

class HorizontalBar
{
	GameEntity* mBarEntity;
	Text* mTextEntity;
	glm::vec2 mScreenCoord;
	std::string mText;
	glm::vec3 mScale;
	std::string mBarTextureName;
	float mBarX;

public:
	HorizontalBar(GameScene* scene, const std::string& text, const glm::vec2& screenCoord, const glm::vec3& scale, const std::string& barTextureName, float barX);
	~HorizontalBar();

	void SetValue(float value);

private:
	void Create(GameScene* scene);
	void CreateText(GameScene* scene);
	void CreateBar(GameScene* scene);
	void UpdateBar(float value);
};

