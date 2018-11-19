#pragma once
template<typename T>
class Singleton
{
public:
	static T& GetInstance();

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	
};

template<typename T>
T& Singleton<T>::GetInstance()
{
	static T mInstance;
	return mInstance;
}

