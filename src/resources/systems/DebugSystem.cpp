#include "stdafx.h"
#include "DebugSystem.h"
#include "src/NGenius.h"
#include "src/resources/IGameEntity.h"
#include "src/input/InputHandler.h"
#include "src/resources/components/DebugComponent.h"
#include "src/resources/components/ColliderDebugComponent.h"
#include "renderSystem/RenderSystem.h"
#include "src/resources/textures/ITexture.h"
#include "src/resources/materials/MaterialsLibrary.h"
#include "src/resources/materials/IMaterial.h"
#include "src/resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "src/resources/materials/effects/MaterialEffectText.h"
#include "src/resources/font/FontType.h"
#include "src/resources/entities/Text.h"
#include "src/statistics/Statistics.h"
#include "GameConstants.h"
#include "src/guiTool/GuiTool.h"
#include "GLFW/glfw3.h"
#include "Memory.h"
#include <algorithm>

const std::vector<std::string> texts = { "FPS: ", "Triangles: ", "Drawcalls: ", "GameEntities(GE): ", "GESpacePartition:", "GERendered:", "GEWithPhysics: ", "DayTime: " };


DebugSystem::DebugSystem(NGenius* engine, RenderSystem* renderSystem, InputHandler* inputHandler) :
	mIsDebugModeEnabled(false),
	mIsBoundingBoxVisible(false),
	mIsColliderVisible(false),
	mIsOverdrawEnabled(false),
	mIsWireframeEnabled(false),
	mIsPostProcessEnabled(true),
	mInputHandler(inputHandler),
	mEngine(engine),
	mRenderSystem(renderSystem),
	mIsInitialized(false)
{
}

DebugSystem::~DebugSystem()
{
	mEntities.clear();
	mInputHandler->UnRegisterInputListener(this);
}

void DebugSystem::Start()
{
	mInputHandler->RegisterAllEventsInputListener(this);
	CreateStatisticsTexts();
	SetTextsVisibility(false);
	mIsInitialized = true;
}

void DebugSystem::Update(float elapsedTime)
{
	assert(mIsInitialized);

	if (mIsDebugModeEnabled)
	{
		if (mIsBoundingBoxVisible || mIsColliderVisible)
		{
			for (IGameEntity* entity : mEntities)
			{
				if (mIsBoundingBoxVisible)
				{
					auto component = entity->GetComponent<DebugComponent>();
					if (component != nullptr && component->IsEnabled())
					{
						auto renderer = component->GetBoundingBoxRenderer();
						mRenderSystem->AddToRender(renderer);
					}
				}
				if (mIsColliderVisible)
				{
					auto component = entity->GetComponent<ColliderDebugComponent>();
					if (component != nullptr && component->IsEnabled())
					{
						auto renderer = component->GetBoundingVolumeRenderer();
						mRenderSystem->AddToRender(renderer);
					}
				}
			}
		}
		
		UpdateStatitstics();
	}
}

void DebugSystem::UpdateStatitstics()
{
	const Statistics* statistics = mEngine->GetStatistics();

	if (materialText != nullptr)
	{
		MaterialEffectText* effect = materialText->GetEffect<MaterialEffectText>();
		int fps = static_cast<int>(statistics->GetNumberFPS());
		if (fps < MIN_FPS_ALLOWED)
		{
			effect->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			effect->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		unsigned int numGameEntities = statistics->GetNumberGameEntities();
		unsigned int numGameEntitiesInsideSpacePartition = statistics->GetNumberGameEntitiesInsideSpacePartition();

		mText[0]->UpdateText(texts[0] + std::to_string(fps));
		mText[1]->UpdateText(texts[1] + std::to_string(statistics->GetNumberTrianglesRendered()));
		mText[2]->UpdateText(texts[2] + std::to_string(statistics->GetNumberDrawCalls()));
		mText[3]->UpdateText(texts[3] + std::to_string(numGameEntities));
		mText[4]->UpdateText(texts[4] + std::to_string(numGameEntitiesInsideSpacePartition));
		mText[5]->UpdateText(texts[5] + std::to_string(statistics->GetNumberRenderers()));
		mText[6]->UpdateText(texts[6] + std::to_string(statistics->GetNumberGameEntitiesWithPhysics()));
		mText[7]->UpdateText(texts[7] + std::to_string(statistics->GetDayTime()));

		//std::cout << "entities rendered: " << statistics->GetNumberRenderers() << "\n";
	}
}

void DebugSystem::SetTextsVisibility(bool visible)
{
	for (size_t i = 0; i < mText.size(); ++i)
	{
		mText[i]->GetRenderer()->SetVisibility(visible);
	}
}

bool DebugSystem::HasDebugComponents(const IGameEntity* entity) const
{
	return entity != nullptr && 
		( entity->HasComponent<DebugComponent>() || entity->HasComponent<ColliderDebugComponent>());
}

void DebugSystem::AddEntity(IGameEntity* entity)
{
	mEntities.push_back(entity);
}

void DebugSystem::RemoveEntity(IGameEntity* entity)
{
	if (HasDebugComponents(entity))
	{
		std::vector<IGameEntity*>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](IGameEntity* a) { return a == entity; });
		if (it != mEntities.end())
		{
			mEntities.erase(it);
		}
		else
		{
			assert(false);
		}
	}
}

void DebugSystem::OnKey(int key, int action)
{
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		mIsBoundingBoxVisible = !mIsBoundingBoxVisible;
		for (IGameEntity* entity : mEntities)
		{
			DebugComponent* component = entity->GetComponent<DebugComponent>();
			if (component != nullptr)
			{
				component->SetBoundingBoxVisibility(mIsBoundingBoxVisible);
			}
		}
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		mIsColliderVisible = !mIsColliderVisible;
		for (IGameEntity* entity : mEntities)
		{
			auto component = entity->GetComponent<ColliderDebugComponent>();
			if (component != nullptr)
			{
				component->SetBoundingVolumeVisibility(mIsColliderVisible);
			}
		}
	}

	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		mIsOverdrawEnabled = !mIsOverdrawEnabled;
		mRenderSystem->SetOverdrawEnabled(mIsOverdrawEnabled);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		mIsWireframeEnabled = !mIsWireframeEnabled;

		glPolygonMode(GL_FRONT_AND_BACK, mIsWireframeEnabled ? GL_LINE : GL_FILL);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		mIsPostProcessEnabled = !mIsPostProcessEnabled;
		mRenderSystem->SetPostProcessEnabled(mIsPostProcessEnabled);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		GuiTool* guiTool = mRenderSystem->GetGuiTool();
		guiTool->IsVisible() ? guiTool->Hide() : guiTool->Show();
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		mEngine->Reload();
	}
}

void DebugSystem::CreateStatisticsTexts()
{
	FontType* font = mEngine->GetFont("OCR A Extended");
	IShaderProgram* shader = mEngine->GetShader("TextShader");
	materialText = mEngine->CreateMaterial("MaterialDebugStatisticsText", shader);
	materialText->AddEffect(DBG_NEW MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	materialText->AddEffect(DBG_NEW MaterialEffectText(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		0.4f,
		0.1f,
		0.0f,
		0.0f,
		glm::vec2(0.0f)));

	GameScene* scene = mEngine->GetGameScene("mainScene");

	for (unsigned int i = 0; i < texts.size(); ++i)
	{
		Text* text = DBG_NEW Text(
			DBG_NEW Transformation(glm::vec3(-mEngine->GetScreenWidth() * 0.5f, mEngine->GetScreenHeight() * 0.5f - (i+1) * 20.0f, 0.0f),
				glm::vec3(0.0f),
				glm::vec3(0.70f)
			),
			materialText, font,
			texts[i], false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);		
		scene->AddEntity(text);
		mText.push_back(text);
	}
}

void DebugSystem::SetDebugModeEnabled(bool enable)
{
	mIsDebugModeEnabled = enable;
	SetTextsVisibility(mIsDebugModeEnabled);
}

bool DebugSystem::IsDebugModeEnabled() const
{
	return mIsDebugModeEnabled;
}

void DebugSystem::OnGameEntityAdded(IGameEntity* entity)
{
	if (HasDebugComponents(entity))
	{
		AddEntity(entity);
	}
}

void DebugSystem::OnGameEntityRemoved(IGameEntity* entity)
{
	if (HasDebugComponents(entity))
	{
		RemoveEntity(entity);
	}
}