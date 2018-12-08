#include "stdafx.h"
#include "HealthHud.h"

#include "../ShooterGameConstants.h"
#include "../../NGenius.h"
#include "HorizontalBar.h"
#include "../../resources/events/characterControllerEvents/HealthEvent.h"

#include "../../resources/materials/IMaterial.h"
#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/GUIRenderer.h"
#include "../../resources/GameEntity.h"
#include "../../resources/scene/GameScene.h"

#include <GL/glew.h>

const std::string GRADIENT_MATERIAL("gradient_material");
const std::string GRADIENT_TEXTURE("gradient");

const std::string HEALTH_BAR_TEXTURE("white_line");
const std::string HEALTH_TEXT("Health:");

static const glm::vec3 HEALTH_HUD_SCALE(440.0f, 128.0f, 1.0f);
static const glm::vec3 HEALTH_BAR_SCALE(200.0f, 15.0f, 1.0f);
static const float HEALTH_BAR_X = 100.0f;

HealthHUD::HealthHUD(GameScene* scene, const glm::vec2& screenCoord) : 
	mScreenCoord(screenCoord),
	mBackgroundEntity(nullptr),
	mHorizontalBar(nullptr)
{
	Create(scene);
}



HealthHUD::~HealthHUD()
{
}

void HealthHUD::Create(GameScene* scene)
{
	CreateBackground(scene);
	CreateHorizontalBar(scene);
}

void HealthHUD::CreateBackground(GameScene* scene)
{
	//background
	IMaterial* material = NGenius::GetInstance().CreateDiffuseGUIMaterial(GRADIENT_MATERIAL, GRADIENT_TEXTURE);
	IRenderer* guiRenderer = new GUIRenderer(NGenius::GetInstance().GetModel(GUI_QUAD_MODEL), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);
	
	mBackgroundEntity = new GameEntity(
		new Transformation(glm::vec3(mScreenCoord.x, mScreenCoord.y, 0.0f),
			glm::vec3(0.0f),
			HEALTH_HUD_SCALE),
		guiRenderer);
	scene->AddEntity(mBackgroundEntity);
}

void HealthHUD::CreateHorizontalBar(GameScene* scene)
{
	mHorizontalBar = new HorizontalBar(scene, HEALTH_TEXT, mScreenCoord, HEALTH_BAR_SCALE, HEALTH_BAR_TEXTURE, HEALTH_BAR_X);
}

void HealthHUD::OnHealthEvent(const HealthEvent* health)
{
	mHorizontalBar->SetValue(health->GetHealth() / health->GetMaxHealth());
}