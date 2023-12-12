#include "STDInc.hpp"

namespace Game
{
	char* sys_processSemaphoreFile = reinterpret_cast<char*>(0x13E1F10);
	//-------------------------------------------------
	Sys_SuspendOtherThreads_t Sys_SuspendOtherThreads = Sys_SuspendOtherThreads_t(0x539E40);
	//-------------------------------------------------
	int	sys_timeBase;
	int Sys_MilliSeconds()
	{
		int	sys_curtime;
		static bool	initialized = false;

		if (!initialized) {
			sys_timeBase = timeGetTime();
			initialized = true;
		}
		sys_curtime = timeGetTime() - sys_timeBase;

		return sys_curtime;
	}

	int Sys_MillisecondsRaw()
	{
		return timeGetTime();
	}

	int Sys_IsDatabaseReady()
	{
		return WaitForSingleObject(Game::databaseCompletedEvent, 0) == 0;
	}

	int Sys_IsDatabaseReady2()
	{
		return WaitForSingleObject(Game::databaseCompletedEvent2, 0) == 0;
	}

	void Sys_CreateConsole/*ax*/(HINSTANCE hInstance /*edi*/)
	{
		const static uint32_t Sys_CreateConsole_func = 0x5962B0;
		__asm
		{
			mov		edi, hInstance;
			call	Sys_CreateConsole_func;
		}
	}

	void Sys_ShowConsole()
	{
		if (!*Game::hWndParent)
		{
			HMODULE ModuleHandleA = GetModuleHandleA(0);
			Game::Sys_CreateConsole(ModuleHandleA);
		}

		ShowWindow(*Game::hWndParent, SW_SHOWNORMAL);
		SendMessageA(*Game::hWndBuffer, 0xB6u, 0, 0xFFFF);
		DeleteFileA(&*Game::sys_processSemaphoreFile);
	}
}