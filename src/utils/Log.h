#pragma once
#include <sstream>

class Log
{
public:
	enum LOG_TYPE
	{
		LOG_ERROR,
		LOG_WARNING,
		LOG_INFO
	};
public:
	Log(LOG_TYPE type);
	Log::~Log();
	
	template <typename T>
	inline Log& operator<<(T const & t)
	{
		//"\033[1;31mbold red text\033[0m normal text\n"
		if (mType == LOG_ERROR || mType == LOG_WARNING)
		{
			mBuffer << "\033[1;31m" << t;
		}
		else
		{
			mBuffer << "\033[0m" << t;
		}
		return *this;
	}
private:
	std::ostringstream mBuffer;
	LOG_TYPE mType;
};

