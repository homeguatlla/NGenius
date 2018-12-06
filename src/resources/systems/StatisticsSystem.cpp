#include "stdafx.h"
#include "StatisticsSystem.h"
#include "../../EngineConstants.h"
#include "../entities/Text.h"
#include "../font/FontType.h"

#include "../Transformation.h"
#include "../scene/GameScene.h"

#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectText.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"

#include "../../statistics/Statistics.h"

#include <string>

const std::vector<std::string> texts = { "FPS: ", "Triangles: ", "Drawcalls: ", "GameEntities(GE): ", "GESpacePartition:", "GERendered:", "GEWithPhysics: ", "DayTime: " };


StatisticsSystem::StatisticsSystem() :
	mTextMaterial(nullptr),
	mIsEnabled(false),
	mIsDirty(true)
{
	
}


StatisticsSystem::~StatisticsSystem()
{
	mTextList.clear();
}

void StatisticsSystem::Start(GameScene* scene, Transformation* transformation, FontType* font, IMaterial* material)
{
	assert(scene != nullptr);
	assert(font != nullptr);
	assert(material != nullptr);
	assert(transformation != nullptr);
	
	mTextMaterial = material;
	mGameScene = scene;
	Create(transformation, font);
}

void StatisticsSystem::Update(const Statistics& statistics)
{
	if (mIsDirty)
	{
		UpdateEnable();
	}

	if (mIsEnabled)
	{
		MaterialEffectText* effect = mTextMaterial->GetEffect<MaterialEffectText>();
		int fps = static_cast<int>(statistics.GetNumberFPS());
		if (fps < EngineConstants::MIN_FPS_ALLOWED)
		{
			effect->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			effect->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		unsigned int numGameEntities = statistics.GetNumberGameEntities();
		unsigned int numGameEntitiesInsideSpacePartition = statistics.GetNumberGameEntitiesInsideSpacePartition();

		mTextList[0]->UpdateText(texts[0] + std::to_string(fps));
		mTextList[1]->UpdateText(texts[1] + std::to_string(statistics.GetNumberTrianglesRendered()));
		mTextList[2]->UpdateText(texts[2] + std::to_string(statistics.GetNumberDrawCalls()));
		mTextList[3]->UpdateText(texts[3] + std::to_string(numGameEntities));
		mTextList[4]->UpdateText(texts[4] + std::to_string(numGameEntitiesInsideSpacePartition));
		mTextList[5]->UpdateText(texts[5] + std::to_string(statistics.GetNumberRenderers()));
		mTextList[6]->UpdateText(texts[6] + std::to_string(statistics.GetNumberGameEntitiesWithPhysics()));
		mTextList[7]->UpdateText(texts[7] + std::to_string(statistics.GetDayTime()));
	}
}

void StatisticsSystem::SetEnable(bool enable)
{
	mIsEnabled = enable;
	mIsDirty = true;
}

void StatisticsSystem::UpdateEnable()
{
	if (mIsDirty)
	{
		for (unsigned int i = 0; i < texts.size(); ++i)
		{
			if (mIsEnabled)
			{
				mGameScene->AddEntity(mTextList[i]);
			}
			else
			{
				mGameScene->RemoveEntity(mTextList[i]);
			}
		}
		mIsDirty = false;
	}
}

void StatisticsSystem::Create(Transformation* transformation, FontType* font)
{
	mTextMaterial->AddEffect(new MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	mTextMaterial->AddEffect(new MaterialEffectText(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		0.4f,
		0.1f,
		0.0f,
		0.0f,
		glm::vec2(0.0f)));

	for (unsigned int i = 0; i < texts.size(); ++i)
	{
		glm::vec3 position = transformation->GetPosition() + glm::vec3(0.0f, 20.0f * i, 0.0f);
		Text* text = new Text(
			new Transformation(position, transformation->GetRotation(), transformation->GetScale()),
			mTextMaterial, font,
			texts[i], false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);

		mTextList.push_back(text);
	}
}