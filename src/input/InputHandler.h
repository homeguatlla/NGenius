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

	void OnKey(int key, int action);
	void OnMouseScroll(float scroll);
};

