#include "stdafx.h"
#include "LoadingState.h"
#include "../../NGenius.h"
#include "../../resources/entities/Text.h"


#include <GLFW\glfw3.h>

const std::string LOADING_TEXT("Loading...");

void LoadingState::OnInit()
{
	mEngine = GetContext()->GetEngine();
}

void LoadingState::OnEnter(float deltaTime)
{
	//GetContext()->GetInformationText()->UpdateText(LOADING_TEXT);
	//GetContext()->GetInformationText()->GetRenderer()->SetVisibility(true);
	mEngine->LoadFromFile(GetContext()->GetFilename());
}

void LoadingState::OnExit(float deltaTime)
{
	//GetContext()->GetInformationText()->GetRenderer()->SetVisibility(false);
}

void LoadingState::OnUpdate(float deltaTime)
{
	GetContext()->GoToState(NGeniusState::STATE_START);
}