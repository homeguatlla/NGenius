#pragma once
#include "../../statistics/Statistics.h"
#include <vector>

class Text;
class IMaterial;

class StatisticsSystem
{
	std::vector<Text*> mTextList;
	IMaterial* mTextMaterial;
	
public:
	StatisticsSystem();
	~StatisticsSystem();

	void Start(GameScene* scene, Transformation* transformation, FontType* font, IMaterial* material);
	void Update(const Statistics& statistics);

private:

	void Create(GameScene* scene, Transformation* transformation, FontType* font);
};

