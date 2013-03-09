#pragma once

struct Change
{
	int *pos;
	int value;

	explicit Change()
	{
		pos = NULL;
		value = 2;
	}

	explicit Change(int *position, const int newValue) : pos(position), value(newValue)
	{
	}

	Change(Change &arg) : pos(arg.pos), value(arg.value)
	{

	}

	Change& operator=(const Change & rhs)
	{
		pos = rhs.pos;
		value = rhs.value;
		return *this;
	}
};