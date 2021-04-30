#pragma once

template <class Owner>
class CStateBase
{
protected:

	Owner m_cOwner;

public:

	CStateBase(Owner _cOwner)
	{
		m_cOwner = _cOwner;
	}

	virtual void Enter() {};
	virtual void Execute() {};
	virtual void Exit() {};

};