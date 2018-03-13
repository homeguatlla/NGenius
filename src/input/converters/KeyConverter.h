#pragma once
#include "../InputEvent.h"
#include "../InputConverter.h"

class KeyConverter : public InputConverter
{
	int mKey;
	int mAction;
	const InputEvent* mEvent;

public:
	KeyConverter(int key, int action, const InputEvent* event);
	~KeyConverter();

	const InputEvent* Convert(int key, int action) const;
};

