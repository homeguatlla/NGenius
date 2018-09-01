#include "stdafx.h"
#include "KeyFrame.h"


KeyFrame::KeyFrame(float timeStamp, std::map<std::string, JointTransform*>& pose) :
	mTimestamp(timeStamp),
	mPose(pose)
{
}

KeyFrame::~KeyFrame()
{
}

float KeyFrame::GetTimestamp() const
{
	return mTimestamp;
}

std::map<std::string, JointTransform*>& KeyFrame::GetJointKeyframes()
{
	return mPose;
}