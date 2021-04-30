#pragma once

#include <list>

#include "GameObject.h"
#include "Transform/D3DTransform.h"


class CD3DGameObject 
	: public CGameObject<CD3DTransform>
{
private:

	static std::list<CD3DGameObject*> m_lObjectList;

	void Constructor()
	{
		m_lObjectList.push_back(this);
	}

	void Destructor()
	{
		for (auto itr = m_lObjectList.begin(); itr != m_lObjectList.end(); itr++)
		{
			if ((*itr) == this)
			{
				m_lObjectList.erase(itr);
				break;
			}
		}
	}

public:

	CD3DGameObject() :CGameObject(){
		Constructor();
	};
	CD3DGameObject(const std::string _Tag) : CGameObject(_Tag) 
	{
		Constructor();
	};
	CD3DGameObject(const std::string _Tag, const std::string _Layer) :CGameObject(_Tag, _Layer) 
	{
		Constructor();
	};

	virtual ~CD3DGameObject()
	{
		Destructor();
	}


};