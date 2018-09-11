#pragma once

#include <vector>
#include <string>

class KeyFrame;

class Animation
{
public:
	Animation(std::string& name, long long duration, std::vector<KeyFrame*>& frames);
	~Animation();

	std::string GetName() const;
	long long GetDuration() const;
	std::vector<KeyFrame*>& GetKeyframes();

private:
	std::string mName;
	long long mDuration; //in seconds
	std::vector<KeyFrame*> mKeyframes;
};

