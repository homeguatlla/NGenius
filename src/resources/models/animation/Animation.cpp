#include "stdafx.h"
#include "Animation.h"

Animation::Animation(std::string& name, long long duration, std::vector<KeyFrame*>& frames) :
	mName(name),
	mDuration(duration),
	mKeyframes(frames)
{
}

Animation::~Animation()
{
}

std::string Animation::GetName() const
{
	return mName;
}

long long Animation::GetDuration() const
{
	return mDuration;
}

std::vector<KeyFrame*>& Animation::GetKeyframes()
{
	return mKeyframes;
}