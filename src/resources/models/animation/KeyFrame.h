#pragma once

#include <map>
#include <string>

class JointTransform;

class KeyFrame
{
public:
	KeyFrame();
	KeyFrame(float timeStamp, std::map<std::string, JointTransform*>& pose);
	~KeyFrame();

	float GetTimestamp() const;
	std::map<std::string, JointTransform*>& GetJointKeyframes();
	void SetTimestamp(float timeStamp);
	void AddJointTransform(std::string& name, JointTransform* pose);

private:
	float mTimestamp;
	std::map<std::string, JointTransform*> mPose;
};

