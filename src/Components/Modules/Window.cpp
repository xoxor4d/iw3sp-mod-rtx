#include "STDInc.hpp"

namespace Components
{
	HWND Window::MainWindow = nullptr;
	BOOL Window::CursorVisible = TRUE;
	std::unordered_map<UINT, Utils::Slot<Window::WndProcCallback>> Window::WndMessageCallbacks;
	Utils::Signal<Window::CreateCallback> Window::CreateSignals;

	int Window::Width()
	{
		return Window::Width(Window::MainWindow);
	}

	int Window::Height()
	{
		return Window::Height(Window::MainWindow);
	}

	int Window::Width(HWND window)
	{
		RECT rect;
		Window::Dimension(window, &rect);
		return (rect.right - rect.left);
	}

	int Window::Height(HWND window)
	{
		RECT rect;
		Window::Dimension(window, &rect);
		return (rect.bottom - rect.top);
	}

	void Window::Dimension(RECT* rect)
	{
		Window::Dimension(Window::MainWindow, rect);
	}

	void Window::Dimension(HWND window, RECT* rect)
	{
		if (rect)
		{
			ZeroMemory(rect, sizeof(RECT));

			if (window && IsWindow(window))
			{
				GetWindowRect(window, rect);
			}
		}
	}

	bool Window::IsCursorWithin(HWND window)
	{
		RECT rect;
		POINT point;
		Window::Dimension(window, &rect);

		GetCursorPos(&point);

		return ((point.x - rect.left) > 0 && (point.y - rect.top) > 0 && (rect.right - point.x) > 0 && (rect.bottom - point.y) > 0);
	}

	HWND Window::GetWindow()
	{
		return Window::MainWindow;
	}

	__declspec(naked) void Window::StyleHookStub()
	{
		const static uint32_t retn_pt = 0x5D9628;
		__asm
		{
			push	eax;
			mov		eax, Dvars::r_noborder;
			cmp		byte ptr[eax + 12], 1;
			pop		eax;

			jne		STOCK;

			mov		ebp, WS_VISIBLE | WS_POPUP;
			jmp		retn_pt;

		STOCK:
			mov		ebp, WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
			jmp		retn_pt;
		}
	}

	int WINAPI Window::ShowCursorHook(BOOL show)
	{
		if (Dvars::ui_nativeCursor->current.enabled && IsWindow(Window::MainWindow) && GetForegroundWindow() == Window::MainWindow && Window::IsCursorWithin(Window::MainWindow))
		{
			static int count = 0;
			(show ? ++count : --count);

			if (count >= 0)
			{
				Window::CursorVisible = TRUE;
			}

			return count;
		}

		return ShowCursor(show);
	}

	HWND WINAPI Window::CreateMainWindow(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
	{
		Window::MainWindow = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

		CreateSignals();

		return Window::MainWindow;
	}

	void Window::ApplyCursor()
	{
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
	}

	BOOL WINAPI Window::MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		if (const auto cb = WndMessageCallbacks.find(Msg); cb != WndMessageCallbacks.end())
		{
			return cb->second(lParam, wParam);
		}

		return Utils::Hook::Call<BOOL(__stdcall)(HWND, UINT, WPARAM, LPARAM)>(0x596810)(hWnd, Msg, wParam, lParam);
	}

	bool Window::IsNoBorder()
	{
		if (Dvars::r_noborder && Dvars::r_noborder->current.enabled)
		{
			return true;
		}

		return false;
	}

	void Window::ExternalConsoleStub()
	{
		if (Dvars::external_console->current.enabled)
			Game::Sys_ShowConsole();

		Utils::Hook::Call<HWND()>(0x5D9980)();
	}

	__declspec(naked) void vid_xypos_stub()
	{
		const static uint32_t retn_addr = 0x5D9915;
		__asm
		{
			mov		[esi + 10h], eax;	// overwritten op (wndParms->y)
			mov		dword ptr[esi], 0;	// overwritten op

			pushad;
			call	Window::IsNoBorder;
			test	al, al;
			jnz		NO_BORDER;

			popad;
			jmp		retn_addr;


		NO_BORDER:
			popad;
			xor		eax, eax;			// clear eax
			mov		[esi + 0Ch], eax;	// set wndParms->x to 0 (4 byte)
			mov		[esi + 10h], eax;	// set wndParms->y to 0 (4 byte)
			jmp		retn_addr;
		}
	}

	Window::Window()
	{
		Dvars::r_noborder = Dvars::Register::Dvar_RegisterBool("r_noborder", "Do not use a border in windowed mode", false, Game::saved);
		Dvars::ui_nativeCursor = Dvars::Register::Dvar_RegisterBool("ui_nativeCursor", "Display native cursor", false, Game::saved);

		// Main window border
		Utils::Hook(0x5D9623, Window::StyleHookStub, HOOK_JUMP).install()->quick();

		// Main window creation
		Utils::Hook::Nop(0x5D968A, 6);
		Utils::Hook(0x5D968A, Window::CreateMainWindow, HOOK_CALL).install()->quick();

		// Don't let the game interact with the native cursor
		Utils::Hook::Set(0x65D2B8, Window::ShowCursorHook);


		// Draw the cursor if necessary
		Scheduler::Loop([]
		{
			if (Dvars::ui_nativeCursor->current.enabled && IsWindow(Window::MainWindow) && GetForegroundWindow() == Window::MainWindow && Window::IsCursorWithin(Window::MainWindow))
			{
				int value = 0;
				Window::ApplyCursor();

				if (Window::CursorVisible)
				{
					while ((value = ShowCursor(TRUE)) < 0) {};
					while (value > 0) { value = ShowCursor(FALSE); } // Set display counter to 0
				}
				else
				{
					while ((value = ShowCursor(FALSE)) >= 0) {};
					while (value < -1) { value = ShowCursor(TRUE); } // Set display counter to -1
				}

				Window::CursorVisible = FALSE;
			}
		}, Scheduler::Pipeline::RENDERER);


		// Use custom message handler
		Utils::Hook::Set(0x595C1E, Window::MessageHandler);

		// Do not use vid_xpos / vid_ypos when r_noborder is enabled
		Utils::Hook::Nop(0x5D990C, 9);
		Utils::Hook(0x5D990C, vid_xypos_stub, HOOK_JUMP).install()->quick();

		Utils::Hook(0x5D9BC9, ExternalConsoleStub, HOOK_CALL).install()->quick();
	}

	Window::~Window()
	{
	}
}