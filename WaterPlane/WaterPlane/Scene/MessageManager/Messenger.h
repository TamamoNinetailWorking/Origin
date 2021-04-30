#pragma once

#include "Delegate.h"

template <class Template,class Interface>
class CMessenger : public CDelegate
{
public:

	typedef void(Template::*EventFunction)(int);

protected:

	Template* m_pObj;
	EventFunction m_hFunction;

public:

	CMessenger() {};
	virtual ~CMessenger() {};

	virtual void operator()(int _value) {
		(m_pObj->*m_hFunction)(_value);
	}

	virtual void operator()(const float _value1, const float _value2, const float _value3, const float _value4) {
		(m_pObj->*m_hFunction)(_value1, _value2, _value3, _value4);
	}

	virtual void operator()(const float _value1, const float _value2) {
		(m_pObj->*m_hFunction)(_value1, _value2);
	}

	void SetFunction(Template* _obj, EventFunction _func) {
		m_pObj = _obj;
		m_hFunction = _func;
	}

	static CDelegate* CreateDelegateor(
		Template* _obj, 
		void(Template::*_func)(int))
	{
		CMessenger* mess = new CMessenger;
		mess->SetFunction(_obj, _func);
		return mess;
	}

	
};