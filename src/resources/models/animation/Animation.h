#pragma once

#include <vector>

class KeyFrame;

class Animation
{
public:
	Animation(long long duration, std::vector<KeyFrame*>& frames);
	~Animation();

	long long GetDuration() const;
	std::vector<KeyFrame*>& GetKeyframes();

private:
	long long mDuration; //in seconds
	std::vector<KeyFrame*> mKeyframes;
};

