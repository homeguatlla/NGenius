#pragma once
#include "../visitor/BaseVisitor.h"
#include "../resources/systems/environmentSystem/EnvironmentSystem.h"

struct GLFWwindow;

class GuiTool : 
	public BaseVisitor,
	public Visitor<EnvironmentSystem>
{
	bool mIsVisible;
	GLFWwindow* mGLFWindow;

	float mSunHour;
	float mSunLightColor[3];
	float mFogGradient;

public:
	GuiTool(GLFWwindow* window);
	~GuiTool();

	void Initialize();
	
	void Show();
	void Hide();
	bool IsVisible() const;

	void Render();

	void Visit(const EnvironmentSystem& environmentSystem);

private:
	void Release();
	void ShowEnvironmentTool();
};

