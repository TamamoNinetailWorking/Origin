#pragma once

#include "../Utility/NonCopyable.h"

//using UINT = unsigned int;
#include <windows.h>

class CDirectoryManager : public NonCopyable
{
private:

	UINT m_directory_name_size;
	char* m_default_current_directory;

public:

	static CDirectoryManager& GetInstance()
	{
		static CDirectoryManager instance;
		return instance;
	}

	virtual ~CDirectoryManager()
	{
		Exit();
	}

	void Init(UINT name_size = 1024)
	{
		m_directory_name_size = name_size;
		m_default_current_directory = new char[m_directory_name_size];

		GetCurrentDirectory(m_directory_name_size, m_default_current_directory);
	};

	void Exit()
	{
		delete[] m_default_current_directory;
	}

	void SetDefaultDirectory()
	{
		SetCurrentDirectory(m_default_current_directory);
	}

	void SetDirectory(const char* directory)
	{
		SetCurrentDirectory(directory);
	}

};