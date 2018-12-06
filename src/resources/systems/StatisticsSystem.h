#pragma once
#include "../../statistics/Statistics.h"
#include <vector>

class Text;
class IMaterial;
class GameScene;

class StatisticsSystem
{
	std::vector<Text*> mTextList;
	IMaterial* mTextMaterial;
	GameScene* mGameScene;
	bool mIsEnabled;
	bool mIsDirty;
	
public:
	StatisticsSystem();
	~StatisticsSystem();

	void Start(GameScene* scene, Transformation* transformation, FontType* font, IMaterial* material);
	void Update(const Statistics& statistics);
	void SetEnable(bool enable);

private:

	void Create(GameScene* scene, Transformation* transformation, FontType* font);
	void UpdateEnable();
};

