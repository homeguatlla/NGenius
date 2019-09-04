#include "stdafx.h"
#include "KeyFrame.h"

KeyFrame::KeyFrame() : 
	mTimestamp(0)
{

}

KeyFrame::KeyFrame(float timeStamp, std::map<std::string, JointTransform*>& pose) :
	mTimestamp(timeStamp),
	mPose(pose)
{
}

KeyFrame::~KeyFrame()
{
	for (auto it = mPose.begin(); it != mPose.end(); ++it)
	{
		delete it->second;
	}
	mPose.clear();
}

float KeyFrame::GetTimestamp() const
{
	return mTimestamp;
}

std::map<std::string, JointTransform*>& KeyFrame::GetJointKeyframes()
{
	return mPose;
}

void KeyFrame::SetTimestamp(float timeStamp)
{
	mTimestamp = timeStamp;
}

void KeyFrame::AddJointTransform(std::string& name, JointTransform* pose)
{
	mPose[name] = pose;
}