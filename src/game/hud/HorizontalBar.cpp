#include "stdafx.h"
#include "HorizontalBar.h"

#include "../ShooterGameConstants.h"
#include "../../NGenius.h"

#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../resources/materials/effects/MaterialEffectText.h"
#include "../../resources/font/FontType.h"
#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/GUIRenderer.h"
#include "../../resources/GameEntity.h"
#include "../../resources/entities/Text.h"
#include "../../resources/scene/GameScene.h"

#include <GL/glew.h>

const std::string BAR_MATERIAL("bar_material");
const std::string TEXT_MATERIAL_NAME("text_material");

HorizontalBar::HorizontalBar(GameScene* scene, const std::string& text, const glm::vec2& screenCoord, const glm::vec3& scale, const std::string& barTextureName, float barX) :
	mScreenCoord(screenCoord),
	mBarEntity(nullptr),
	mTextEntity(nullptr),
	mText(text),
	mScale(scale),
	mBarTextureName(barTextureName),
	mBarX(barX)
{
	Create(scene);
}



HorizontalBar::~HorizontalBar()
{
}

void HorizontalBar::Create(GameScene* scene)
{
	CreateText(scene);
	CreateBar(scene);
}

void HorizontalBar::CreateText(GameScene* scene)
{
	FontType* font = NGenius::GetInstance().GetFont("Quartz MS");

	IMaterial* materialText = NGenius::GetInstance().CreateMaterial(TEXT_MATERIAL_NAME, NGenius::GetInstance().GetShader("text"));
	materialText->AddEffect(new MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	materialText->AddEffect(new MaterialEffectText(
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		0.4f,
		0.1f,
		0.0f,
		0.0f,
		glm::vec2(0.0f)));

	//TODO posicionar el texto en relación a su tamaño quitar el -90 - 50 y calcularlo todo bien

	mTextEntity = new Text(
		new Transformation(glm::vec3(mScreenCoord.x - 90.0f, mScreenCoord.y - 50.0f, 1.0f),
			glm::vec3(0.0f),
			glm::vec3(0.20f)
		),
		materialText, font,
		mText, false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);
	GUIRenderer* renderer = static_cast<GUIRenderer*>(mTextEntity->GetRenderer());
	renderer->SetBlendFactors(GL_SRC_ALPHA, GL_CONSTANT_ALPHA);
	renderer->SetDepthTestEnabled(true);
	scene->AddEntity(mTextEntity);
}

void HorizontalBar::CreateBar(GameScene* scene)
{
	IMaterial* material = NGenius::GetInstance().CreateDiffuseGUIMaterial(BAR_MATERIAL, mBarTextureName);
	IRenderer* guiRenderer = new GUIRenderer(NGenius::GetInstance().GetModel(GUI_QUAD_MODEL), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	//TODO posicionar el texto en relación a su tamaño quitar el -35 y calcularlo todo bien

	mBarEntity = new GameEntity(
		new Transformation(glm::vec3(mScreenCoord.x + mBarX, mScreenCoord.y - 35.0f, 0.0f),
			glm::vec3(0.0f),
			mScale),
		guiRenderer);
	scene->AddEntity(mBarEntity);
}

void HorizontalBar::UpdateBar(float value)
{
	Transformation* transformation = mBarEntity->GetTransformation();

	glm::vec3 scale = transformation->GetScale();
	scale.x = mScale.x * value;
	transformation->SetScale(scale);

	glm::vec3 position = transformation->GetPosition();
	float barX = mScreenCoord.x + mBarX;
	position.x = barX - (mScale.x - scale.x) * 0.5f;
	transformation->SetPosition(position);
}

void HorizontalBar::SetValue(float value)
{
	assert(value >= 0.0f && value <= 1.0f);

	UpdateBar(value);
}