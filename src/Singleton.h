#pragma once
template<typename T>
class Singleton
{
public:
	static T& GetInstance();
private:
	Singleton() {}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	virtual ~Singleton() {}
};

template<typename T>
T& Singleton<T>::GetInstance()
{
	static T mInstance;
	return *mInstance;
}

