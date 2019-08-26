#pragma once
#include <memory>
#include <string>
#include "states/StateTypes.h"

class NGenius;
class Text;

class FSMContext
{
public:
	FSMContext(std::shared_ptr<NGenius> nGenius) : 
		mEngine(nGenius),
		mIsFreeCameraModeEnabled(false),
		mIsFreePlayerModeEnabled(false),
		mInformationText(nullptr) {}

	virtual ~FSMContext() = default;
	void Release();

	std::shared_ptr<NGenius> GetEngine() { return mEngine; }

	void SetEnableFreeModeCamera(bool enable) { mIsFreeCameraModeEnabled = enable; }
	bool IsFreeCameraModeEnabled() const { return mIsFreeCameraModeEnabled; }

	void SetEnableFreeModePlayer(bool enable) { mIsFreePlayerModeEnabled = enable; }
	bool IsFreePlayerModeEnabled() const { return mIsFreePlayerModeEnabled; }
	NGeniusState GetNextState() const { return mNextState; }
	void GoToState(NGeniusState state) { mNextState = state; }
	void ResetFreeMode();	
	Text* GetInformationText();

	void SetFilename(const std::string& filename) { mFilename = filename; }
	const std::string GetFilename() const { return mFilename; }

private:
	void CreateStateUI();

private:
	std::shared_ptr<NGenius> mEngine;
	NGeniusState mNextState;
	bool mIsFreeCameraModeEnabled;
	bool mIsFreePlayerModeEnabled;
	std::string mFilename;

	Text* mInformationText;
};

