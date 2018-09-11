#include "stdafx.h"
#include "KeyFrame.h"

KeyFrame::KeyFrame() : 
	mTimestamp(0)
{

}

KeyFrame::KeyFrame(long long timeStamp, std::map<std::string, JointTransform*>& pose) :
	mTimestamp(timeStamp),
	mPose(pose)
{
}

KeyFrame::~KeyFrame()
{
}

long long KeyFrame::GetTimestamp() const
{
	return mTimestamp;
}

std::map<std::string, JointTransform*>& KeyFrame::GetJointKeyframes()
{
	return mPose;
}

void KeyFrame::SetTimestamp(long long timeStamp)
{
	mTimestamp = timeStamp;
}

void KeyFrame::AddJointTransform(std::string& name, JointTransform* pose)
{
	mPose[name] = pose;
}