#include "STDInc.hpp"

namespace Utils
{
	void OutputDebugLastError()
	{
		DWORD errorMessageID = ::GetLastError();
		OutputDebugStringA(Utils::String::VA("Last error code: 0x%08X (%s)\n", errorMessageID, GetLastWindowsError().data()));
	}

	std::string GetLastWindowsError()
	{
		DWORD errorMessageID = ::GetLastError();
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);
		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);
		return message;
	}

	void SetLegacyEnvironment()
	{
		wchar_t binaryPath[512]{};
		GetModuleFileNameW(GetModuleHandleW(nullptr), binaryPath, sizeof(binaryPath) / sizeof(wchar_t));

		auto* exeBaseName = std::wcsrchr(binaryPath, L'\\');
		exeBaseName[0] = L'\0';

		// Make the game work without the AlterWare launcher
		SetCurrentDirectoryW(binaryPath);
	}

	void SetEnvironment()
	{
		char* buffer{};
		std::size_t size{};
		if (_dupenv_s(&buffer, &size, "COD4_INSTALL") != 0 || buffer == nullptr)
		{
			SetLegacyEnvironment();
			return;
		}

		const auto _0 = gsl::finally([&] { std::free(buffer); });

		SetCurrentDirectoryA(buffer);
		SetDllDirectoryA(buffer);
	}

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