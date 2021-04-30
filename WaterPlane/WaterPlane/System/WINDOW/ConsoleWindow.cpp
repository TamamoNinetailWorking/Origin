#include "CWindow.h"
#include <io.h>
#include <Fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)

void CWINDOW::CreateConsoleWindow()
{
	AllocConsole();
	/*m_hConsole = _open_osfhandle(
		(long)GetStdHandle(STD_OUTPUT_HANDLE),
		_O_TEXT);*/
	//*stdout = *_fdopen(m_hConsole, "w");
	//setvbuf(stdout, NULL, _IONBF, 0);
	freopen("CONOUT$", "w", stdout);
	return;
}

void CWINDOW::CloseConsoleWindow()
{
	//_close(m_hConsole);
	FreeConsole();
	return;
}