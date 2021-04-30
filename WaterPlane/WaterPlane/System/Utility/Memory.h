#pragma once

//�f���[�g�}�N���̃C�����C���֐�
template <class T>
inline void Delete(T*& p)
{
	if (p)
	{
		delete p;
	}
	p = nullptr;
}

template <class T>
inline void Release(T*& p)
{
	if (p) 
	{
		p->Release();
	}
	p = nullptr;
}

//�����[�X�}�N���@�z��p
template <class T>
inline void DeleteArray(T*& p)
{
	if (p) 
	{
		delete[] p;
	}
	p = nullptr;
}


#include <list>
#include <vector>

//���X�g����Y���v�f���폜����
template <class T>
void DeleteListElem(std::list<T*>& arr, T* elem)
{
	typename std::list<T*>::iterator it;
	for (it = arr.begin(); it != arr.end();)
	{
		if ((*it) == elem)
		{
			//���X�g����폜
			Delete(*it);
			it = arr.erase(it);
		}
		else
		{
			++it;//����T��
		}
	}
}

//���X�g���̂�S�č폜
template <class T>
void DeleteList(std::list<T*>& arr)
{
	typename std::list<T*>::iterator it;
	for (it = arr.begin(); it != arr.end(); it++)
	{
		Delete(*it);
	}
	arr.clear();
}

template <class Tempus>
void ReleaseVector(std::vector<Tempus>& arr)
{
	for (auto itr = arr.begin(); itr != arr.end(); std::next(itr))
	{
		Release(*itr);
	}
	arr.clear();
}

template <class Tempus>
void ReleaseList(std::list<Tempus>& arr)
{
	for (auto itr = arr.begin(); itr != arr.end(); std::next(itr))
	{
		Release(*itr);
	}
	arr.clear();
}

//16�o�C�g�ɃA���C�����g���邽�߂̃}�N��
#define ALIGN16 _declspec(align(16))