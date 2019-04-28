#pragma once
#include <map>
#include <string>

template<class T> 
class BaseLibrary
{
protected:
	std::map<const std::string, T> mElements;

public:
	BaseLibrary() {}
	virtual ~BaseLibrary() 
	{
		std::map<const std::string, T>::iterator it;

		for (it = mElements.begin(); it != mElements.end(); ++it)
		{
			delete it->second;
		}
		mElements.clear();
	}

	typedef typename std::map<const std::string, T>::iterator BaseLibraryIterator;

	T GetElement(const std::string& key)
	{
		if (mElements.find(key) != mElements.end())
		{
			return mElements[key];
		}
		else
		{
			return nullptr;
		}
	}

	bool HasElement(const std::string& key) const
	{
		return mElements.find(key) != mElements.end();
	}

	void AddElement(const std::string& key, T element)
	{
		if (mElements.find(key) == mElements.end())
		{
			mElements[key] = element;
		}
	}
};

