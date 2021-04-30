#pragma once

#include "StateBase.h"

template <class Owner>
class CStateMachine
{
private:

	CStateBase<Owner> m_cCurrentState;

public:

	CStateMachine()
	{
		m_cCurrentState = nullptr;
	}

	void ChangeState(CStateBase<Owner> _cState)
	{
		if (m_cCurrentState != nullptr)
		{
			m_cCurrentState.Exit();
		}
		m_cCurrentState = _cState;
		m_cCurrentState.Enter();
	}

	void EndState()
	{
		if (m_cCurrentState != nullptr)
		{
			m_cCurrentState.Exit();
		}
		m_cCurrentState = nullptr;
	}

	void Update()
	{
		if (m_cCurrentState != nullptr)
		{
			m_cCurrentState.Execute();
		}
	}

};