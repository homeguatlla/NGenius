#pragma once

#include <vector>
#include <string>

class KeyFrame;

class Animation
{
public:
	Animation(std::string& name, float duration, std::vector<KeyFrame*>& frames);
	~Animation();

	std::string GetName() const;
	float GetDuration() const;
	std::vector<KeyFrame*>& GetKeyframes();
	
	/*
	bool IsPlaying() const;
	void Play();
	void Stop();
	*/

private:
	std::string mName;
	float mDuration; //in seconds
	std::vector<KeyFrame*> mKeyframes;
	//bool mIsPlaying;
};

