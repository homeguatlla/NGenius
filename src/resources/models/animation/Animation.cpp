#include "stdafx.h"
#include "Animation.h"


Animation::Animation(float duration, std::vector<KeyFrame*>& frames) :
	mDuration(duration),
	mKeyframes(frames)
{
}

Animation::~Animation()
{
}

float Animation::GetDuration() const
{
	return mDuration;
}

std::vector<KeyFrame*>& Animation::GetKeyframes()
{
	return mKeyframes;
}