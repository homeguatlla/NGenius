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

public:
	GuiTool(GLFWwindow* window);
	~GuiTool();

	void Initialize();
	
	void Show();
	void Hide();
	bool IsVisible() const;

	void Render();

	void Visit(EnvironmentSystem& environmentSystem);

private:
	void Release();
	void ShowEnvironmentTool();
};

