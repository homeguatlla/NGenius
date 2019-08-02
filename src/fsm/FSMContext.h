#pragma once
#include <memory>

class NGenius;
class Text;

class FSMContext
{
public:
	FSMContext(std::shared_ptr<NGenius> nGenius) : 
		mEngine(nGenius),
		mIsFreeCameraModeEnabled(false),
		mIsFreePlayerModeEnabled(false),
		mFreeModeText(nullptr) {}

	virtual ~FSMContext() = default;
	void Release();

	std::shared_ptr<NGenius> GetEngine() { return mEngine; }

	void SetEnableFreeModeCamera(bool enable) { mIsFreeCameraModeEnabled = enable; }
	bool IsFreeCameraModeEnabled() const { return mIsFreeCameraModeEnabled; }

	void SetEnableFreeModePlayer(bool enable) { mIsFreePlayerModeEnabled = enable; }
	bool IsFreePlayerModeEnabled() const { return mIsFreePlayerModeEnabled; }

	void ResetFreeMode();	
	Text* GetFreeModeText();

private:
	void CreateFreeModeStateUI();

private:
	std::shared_ptr<NGenius> mEngine;
	bool mIsFreeCameraModeEnabled;
	bool mIsFreePlayerModeEnabled;

	Text* mFreeModeText;
};

