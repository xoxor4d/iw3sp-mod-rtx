#include "STDInc.hpp"

namespace Components
{
	bool Singleton::FirstInstance = true;

	bool Singleton::IsFirstInstance()
	{
		return FirstInstance;
	}

	Singleton::Singleton()
	{
		FirstInstance = (CreateMutexA(nullptr, FALSE, "iw3sp_mod_mutex") && GetLastError() != ERROR_ALREADY_EXISTS);

		if (!FirstInstance && MessageBoxA(nullptr, "Do you want to start another instance?\nNot all features will be available!", "Game already running", MB_ICONEXCLAMATION | MB_YESNO) == IDNO)
		{
			ExitProcess(EXIT_SUCCESS);
		}
	}
}