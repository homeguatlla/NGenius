#pragma once
#include <functional>
#include <vector>

struct GLFWwindow;
class IInputListener;

class InputHandler
{
	typedef std::vector<IInputListener*>::iterator ListenersIterator;
	std::vector<IInputListener*> mListeners;
	GLFWwindow* mWindow;
	std::function<void(GLFWwindow* window)> mInputHandler;
	bool mIsInitialized;

public:
	InputHandler();
	~InputHandler();

	void Init(GLFWwindow* window);

	void Update(float deltaTime);

	void RegisterAllEventsInputListener(IInputListener* listener);
	void UnRegisterInputListener(IInputListener* listener);

	void RegisterInputHandler(std::function<void(GLFWwindow* window)> callback);
	static void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
	static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void OnMouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	
private:
	void Key(int key, int action);
	void MouseScroll(int button, float scroll);
	void MouseButton(int button, int action, int mods);
	void MouseCursorPos(double x, double y);
};

