#pragma once

#include <vector>

class KeyFrame;

class Animation
{
public:
	Animation(float duration, std::vector<KeyFrame*>& frames);
	~Animation();

	float GetDuration() const;
	std::vector<KeyFrame*>& GetKeyframes();

private:
	float mDuration; //in seconds
	std::vector<KeyFrame*> mKeyframes;
};

