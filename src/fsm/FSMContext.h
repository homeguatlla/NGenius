#pragma once
#include <memory>

class NGenius;

class FSMContext
{
public:
	FSMContext(std::shared_ptr<NGenius> nGenius) : mEngine(nGenius) {}
	virtual ~FSMContext() = default;

	std::shared_ptr<NGenius> GetEngine() { return mEngine; }

private:
	std::shared_ptr<NGenius> mEngine;
};

