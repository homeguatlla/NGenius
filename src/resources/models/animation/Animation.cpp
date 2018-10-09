#include "stdafx.h"
#include "Animation.h"

Animation::Animation(std::string& name, float duration, std::vector<KeyFrame*>& frames) :
	mName(name),
	mDuration(duration),
	mKeyframes(frames)/*,
	mIsPlaying(false)*/
{
}

Animation::~Animation()
{
}

std::string Animation::GetName() const
{
	return mName;
}

float Animation::GetDuration() const
{
	return mDuration;
}

std::vector<KeyFrame*>& Animation::GetKeyframes()
{
	return mKeyframes;
}
/*
void Animation::Play()
{
	mIsPlaying = true;
}

void Animation::Stop()
{
	mIsPlaying = false;
}

bool Animation::IsPlaying() const
{
	return mIsPlaying;
}*/