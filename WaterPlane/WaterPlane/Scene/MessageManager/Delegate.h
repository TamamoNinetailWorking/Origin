#pragma once

class CDelegate
{
public:

	CDelegate() {};
	virtual ~CDelegate() {};
	virtual void operator()(int value) = 0;

	virtual void operator()(const float value1, const float value2, const float value3, const float value4) = 0;
	virtual void operator()(const float value1, const float value2) = 0;
};