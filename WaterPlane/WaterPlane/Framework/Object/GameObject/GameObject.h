#pragma once

#include <string>
#include <list>
#include "../../../System/Utility/Memory.h"

template <class Transform>
class CGameObject
{
private:

	///<summary>
	///後々はこのオブジェクトリストの機能自体を
	///オブジェクトマネージャーとして分離するのが良いだろう
	///</summary>
	//static std::list<CGameObject*> m_lObjectList;

protected:

	Transform m_cTransform;
	std::string m_sTag;
	std::string m_sLayer;

	bool m_bActivate;

	void CommonInit() {
		m_bActivate = false;
	}

	void ConstructDefaultProcess(const std::string _Tag, const std::string _Layer)
	{
		CommonInit();
		m_sTag = _Tag;
		m_sLayer = _Layer;
		//m_lObjectList.push_back(this);
	}

public:

	CGameObject() 
	{
		ConstructDefaultProcess(std::string("Default"), std::string("Default"));
	};

	CGameObject(const std::string _Tag)
	{
		ConstructDefaultProcess(_Tag, std::string("Default"));
	}

	CGameObject(const std::string _Tag, std::string _Layer)
	{
		ConstructDefaultProcess(_Tag, _Layer);
	}

	virtual ~CGameObject() 
	{
		/*for (auto itr = m_lObjectList.begin(); itr != m_lObjectList.end(); itr++){
			if ((*itr) == this){
				m_lObjectList.erase(itr);
				break;
			}
		}*/
	};

	Transform GetTransform(){
		return m_cTransform;
	}

	std::string GetObjectTag() const {
		return m_sTag;
	}

	std::string GetObjectLayer() const {
		return m_sLayer;
	}

	void SetActivation(bool _flag) {
		m_bActivate = _flag;
	}

	virtual void Start() {};

	virtual void Update() {
		if (!m_bActivate){
			Start();
			m_bActivate = true;
		}
	};

	virtual void Draw() {};

	virtual void Exit() {};

};