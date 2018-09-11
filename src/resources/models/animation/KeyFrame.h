#pragma once

#include <map>
#include <string>

class JointTransform;

class KeyFrame
{
public:
	KeyFrame();
	KeyFrame(long long timeStamp, std::map<std::string, JointTransform*>& pose);
	~KeyFrame();

	long long GetTimestamp() const;
	std::map<std::string, JointTransform*>& GetJointKeyframes();
	void SetTimestamp(long long timeStamp);
	void AddJointTransform(std::string& name, JointTransform* pose);

private:
	long long mTimestamp;
	std::map<std::string, JointTransform*> mPose;
};

