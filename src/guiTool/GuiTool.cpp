#include "stdafx.h"
#include "GuiTool.h"

#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"


GuiTool::GuiTool(GLFWwindow* window) :
	mIsVisible(false),
	mGLFWindow(window),
	mSunHour(12.0f)
{
	for (int i = 0; i < 3; ++i)
	{
		mSunLightColor[i] = 1.0f;
	}
}

GuiTool::~GuiTool()
{
	Release();
}

void GuiTool::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.WantCaptureKeyboard = false;
	//io.WantCaptureMouse = true;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(mGLFWindow, false); //false = no opengl keyboard, mouse callbacks
	ImGui_ImplOpenGL3_Init();
	
	// Setup style
	ImGui::StyleColorsDark();
}

void GuiTool::Render()
{
	if (mIsVisible)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ShowEnvironmentTool();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

void GuiTool::Release()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GuiTool::Show()
{
	mIsVisible = true;
	glfwSetInputMode(mGLFWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GuiTool::Hide()
{
	mIsVisible = false;
	glfwSetInputMode(mGLFWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool GuiTool::IsVisible() const
{
	return mIsVisible;
}

void GuiTool::ShowEnvironmentTool()
{
	ImGui::Begin("Environment:");
	ImGui::Text("Sun time");
	ImGui::SliderFloat("hour:", &mSunHour, 0.0, 24.0);
	ImGui::ColorPicker3("sun light color:", &mSunLightColor[0]);

	if (ImGui::Button("Save"))
	{
		// do stuff
	}
	ImGui::End();
}

void GuiTool::Visit(EnvironmentSystem& environmentSystem)
{
	environmentSystem.SetDayHour(mSunHour);
	environmentSystem.SetSunLightColor(glm::vec3(mSunLightColor[0], mSunLightColor[1], mSunLightColor[2]));
}