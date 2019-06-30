#pragma once
#include <memory>

class NGenius;

class FSMContext
{
public:
	FSMContext(std::shared_ptr<NGenius> nGenius) : mNGenius(nGenius) {}
	virtual ~FSMContext() = default;

	std::shared_ptr<NGenius> GetNGenius() { return mNGenius; }

private:
	std::shared_ptr<NGenius> mNGenius;
};

