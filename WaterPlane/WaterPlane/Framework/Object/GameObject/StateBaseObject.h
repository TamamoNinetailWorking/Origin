#pragma once

#include "D3DGameObject.h"
#include "StateMachine/StateBase.h"
#include "StateMachine/StateMachine.h"

#include <list>

template <enum State>
class CStateBaseObject : public CD3DGameObject
{
protected:

	std::list<CStateBase<CStateBaseObject*>> m_cStateList;
	
	std::list<CStateMachine<CStateBaseObject*>> m_cStateMachineList;

public:

	//StateChange
	//	index	StateMachineIndex
	//	state	StateBaseIndex
	virtual void ChangeState(int index, State state)
	{
		if (m_cStateMachineList.size <= index) {
			return;
		}
		m_cStateMachineList[index].ChangeState(m_cStateList[(int)state]);
	}

	virtual void EndState(int index)
	{
		if (m_cStateMachineList.size <= index){
			return;
		}
		m_cStateMachineList[index].EndState();
	}

	virtual bool IsCurrentState(int index, State state)
	{
		if (m_cStateMachineList.size <= index) {
			return false;
		}
		return m_cStateMachineList[index].GetCurrentState() == m_cStateList[(int)state];
	}

	virtual void Update() override
	{
		for (auto itr = m_cStateMachineList.begin();itr != m_cStateMachineList.end();itr++)
		{
			itr->Update();
		}
	}
};