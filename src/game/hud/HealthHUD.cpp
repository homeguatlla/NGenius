#include "stdafx.h"
#include "HealthHud.h"

#include "../ShooterGameConstants.h"
#include "../../NGenius.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../resources/materials/effects/MaterialEffectText.h"
#include "../../resources/font/FontType.h"
#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/IndicesRenderer.h"
#include "../../resources/renderers/ParticleRenderer.h"
#include "../../resources/renderers/GUIRenderer.h"
#include "../../resources/GameEntity.h"
#include "../../resources/entities/Text.h"
#include "../../resources/scene/GameScene.h"

#include "../../utils/Log.h"
#include <GL/glew.h>

const std::string GRADIENT_MATERIAL("gradient_material");
const std::string GRADIENT_TEXTURE("gradient");
const std::string HEALTH_TEXT("Health:");
const std::string HEALTH_MATERIAL_NAME("health_material");

static const glm::vec3 HEALTH_HUD_SCALE(300.0f, 128.0f, 1.0f);


int factorD;
int factorS;
const int factors[14] = { GL_ZERO,
							GL_ONE,
							GL_SRC_COLOR,
							GL_ONE_MINUS_SRC_COLOR,
							GL_DST_COLOR,
							GL_ONE_MINUS_DST_COLOR,
							GL_SRC_ALPHA,
							GL_ONE_MINUS_SRC_ALPHA,
							GL_DST_ALPHA,
							GL_ONE_MINUS_DST_ALPHA,
							GL_CONSTANT_COLOR,
							GL_ONE_MINUS_CONSTANT_COLOR,
							GL_CONSTANT_ALPHA,
							GL_ONE_MINUS_CONSTANT_ALPHA
};

HealthHUD::HealthHUD(GameScene* scene, const glm::vec2& screenCoord) : mScreenCoord(screenCoord)
{
	Create(scene);
}



HealthHUD::~HealthHUD()
{
}

void HealthHUD::Create(GameScene* scene)
{
	CreateBackground(scene);
	CreateText(scene);
}

void HealthHUD::CreateBackground(GameScene* scene)
{
	//background
	IMaterial* material = NGenius::GetInstance().CreateDiffuseGUIMaterial(GRADIENT_MATERIAL, GRADIENT_TEXTURE);
	IRenderer* guiRenderer = new GUIRenderer(NGenius::GetInstance().GetModel(GUI_QUAD_MODEL), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);
	//guiRenderer->SetTransparency(true);

	mBackgroundEntity = new GameEntity(
		new Transformation(glm::vec3(mScreenCoord.x, mScreenCoord.y, 0.0f),
			glm::vec3(0.0f),
			HEALTH_HUD_SCALE),
		guiRenderer);
	//GUIRenderer* renderer = static_cast<GUIRenderer*>(mBackgroundEntity->GetRenderer());
	//renderer->SetBlendFactors(GL_SRC_ALPHA, GL_DST_COLOR);
	scene->AddEntity(mBackgroundEntity);
}

void HealthHUD::UpdateValues()
{
	GUIRenderer* renderer = static_cast<GUIRenderer*>(mHealthTextEntity->GetRenderer());
	renderer->SetBlendFactors(GL_SRC_ALPHA, factors[factorS]);

	//renderer = static_cast<GUIRenderer*>(mBackgroundEntity->GetRenderer());
	//renderer->SetBlendFactors(factors[factorS], GL_DST_COLOR);

	Log(Log::LOG_INFO) << "factorS " << factorS << " factorD " << factorD << "\n";

	factorS = factorS + 1;
	if (factorS > 13)
	{
		factorS = 0;
		factorD++;
	}
}

void HealthHUD::CreateText(GameScene* scene)
{
	FontType* font = NGenius::GetInstance().GetFont("Quartz MS");

	IMaterial* materialText = NGenius::GetInstance().CreateMaterial(HEALTH_MATERIAL_NAME, NGenius::GetInstance().GetShader("text"));
	materialText->AddEffect(new MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	materialText->AddEffect(new MaterialEffectText(
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		0.4f,
		0.1f,
		0.0f,
		0.0f,
		glm::vec2(0.0f)));

	mHealthTextEntity = new Text(
		new Transformation(glm::vec3(mScreenCoord.x - 80.0f, mScreenCoord.y - 50.0f, 1.0f),
			glm::vec3(0.0f),
			glm::vec3(0.20f)
		),
		materialText, font,
		HEALTH_TEXT, false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);
	GUIRenderer* renderer = static_cast<GUIRenderer*>(mHealthTextEntity->GetRenderer());
	renderer->SetBlendFactors(GL_SRC_ALPHA, GL_CONSTANT_ALPHA);
	renderer->SetDepthTestEnabled(true);
	scene->AddEntity(mHealthTextEntity);
}