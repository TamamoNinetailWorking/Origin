#pragma once

//デリートマクロのインライン関数
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

//リリースマクロ　配列用
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

//リストから該当要素を削除する
template <class T>
void DeleteListElem(std::list<T*>& arr, T* elem)
{
	typename std::list<T*>::iterator it;
	for (it = arr.begin(); it != arr.end();)
	{
		if ((*it) == elem)
		{
			//リストから削除
			Delete(*it);
			it = arr.erase(it);
		}
		else
		{
			++it;//次を探索
		}
	}
}

//リスト自体を全て削除
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

//16バイトにアライメントするためのマクロ
#define ALIGN16 _declspec(align(16))