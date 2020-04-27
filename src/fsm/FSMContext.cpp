#include "stdafx.h"
#include "FSMContext.h"
#include "../resources/materials/MaterialsLibrary.h"
#include "../resources/materials/IMaterial.h"
#include "../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../resources/materials/effects/MaterialEffectText.h"
#include "../resources/entities/Text.h"
#include "../resources/font/FontType.h"
#include "../resources/renderers/IRenderer.h"
#include "../NGenius.h"
#include "../GameConstants.h"


void FSMContext::Release()
{
	mInformationText = nullptr;
}

void FSMContext::ResetFreeMode()
{
	mIsFreeCameraModeEnabled = false;
	mIsFreePlayerModeEnabled = false;
}

std::shared_ptr<Text> FSMContext::GetInformationText()
{
	if (mInformationText == nullptr)
	{
		CreateStateUI();
	}

	return mInformationText;
}

void FSMContext::CreateStateUI()
{
	FontType* font = mEngine->GetFont("OCR A Extended");
	IShaderProgram* shader = mEngine->GetShader("TextShader");
	IMaterial* materialText = mEngine->CreateMaterial("MaterialFreeModeText", shader);
	materialText->AddOrReplaceEffect(DBG_NEW MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	materialText->AddOrReplaceEffect(DBG_NEW MaterialEffectText(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		0.4f,
		0.1f,
		0.0f,
		0.0f,
		glm::vec2(0.0f)));

	mInformationText = std::make_shared<Text>(
		DBG_NEW Transformation(
			glm::vec3(mEngine->GetScreenWidth() * 0.4f, -mEngine->GetScreenHeight() * 0.47f, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.7f)
		),
		materialText, font,
		TEXT_FREE_MODE, false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);
	mInformationText->SetName("InformationText");
	mEngine->AddEntity(mInformationText);
}