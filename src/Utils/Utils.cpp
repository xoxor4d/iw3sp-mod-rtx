#include "STDInc.hpp"

namespace Utils
{
	void SafeShellExecute(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd)
	{
		[=]()
		{
			__try
			{
				ShellExecuteA(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
			}
			__finally
			{
			}
		}();

		std::this_thread::yield();
	}

	void OpenUrl(const std::string& url)
	{
		SafeShellExecute(nullptr, "open", url.data(), nullptr, nullptr, SW_SHOWNORMAL);
	}
}