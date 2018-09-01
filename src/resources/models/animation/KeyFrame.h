#pragma once

#include <map>
#include <string>

class JointTransform;

class KeyFrame
{
public:
	KeyFrame(float timeStamp, std::map<std::string, JointTransform*>& pose);
	~KeyFrame();

	float GetTimestamp() const;
	std::map<std::string, JointTransform*>& GetJointKeyframes();

private:
	float mTimestamp;
	std::map<std::string, JointTransform*> mPose;
};

