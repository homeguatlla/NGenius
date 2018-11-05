#pragma once
#include <sstream>

//https://gist.github.com/vratiu/9780109

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
	void Reset();

	template <typename T>
	inline Log& operator<<(T const & t)
	{
		//"\033[1;31mbold red text\033[0m normal text\n"
		//"\033[A;BBm\" where:
		//A = 0 normal, 1, bold, 4 underline, 
		//first B 3 text, 4 background, 9 high intensity, 10 background high intensity
		//second B color, 0 black, 1 red, 2 green, 3 yellow, 4 blue, 5 purple, 6 cyan, 7 white
		//"\[\033[0m\]" to reset
		switch (mType)
		{
			case LOG_ERROR:
				mBuffer << "\033[0;91m" << t;
				break;
			case LOG_WARNING:
				mBuffer << "\033[0;95m" << t;
				break;
			default:
				mBuffer << "\033[0m" << t;
				break;
		}
		
		return *this;
	}
private:
	std::ostringstream mBuffer;
	LOG_TYPE mType;
};

