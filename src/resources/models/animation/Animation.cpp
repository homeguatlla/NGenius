#include "stdafx.h"
#include "Animation.h"


Animation::Animation(long long duration, std::vector<KeyFrame*>& frames) :
	mDuration(duration),
	mKeyframes(frames)
{
}

Animation::~Animation()
{
}

long long Animation::GetDuration() const
{
	return mDuration;
}

std::vector<KeyFrame*>& Animation::GetKeyframes()
{
	return mKeyframes;
}