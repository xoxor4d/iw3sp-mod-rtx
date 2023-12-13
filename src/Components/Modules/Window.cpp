#include "STDInc.hpp"

IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	void Window::OnWndMessage(UINT Msg, Utils::Slot<Window::WndProcCallback> callback)
	{
		WndMessageCallbacks.emplace(Msg, callback);
	}

	void Window::OnCreate(Utils::Slot<CreateCallback> callback)
	{
		CreateSignals.connect(callback);
	}

	__declspec(naked) void Window::StyleHookStub()
	{
		const static uint32_t retn_pt = 0x5D9628;
		__asm
		{
			call	Window::IsNoBorder;
			test	al, al

			jz		SetBorder

			mov		ebp, WS_VISIBLE | WS_POPUP;
			jmp		retn_pt;

		SetBorder:
			mov		ebp, WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
			jmp		retn_pt;
		}
	}

	void Window::DrawCursor(const float* color, Game::ScreenPlacement* ScrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, Game::Material* material)
	{
		if (Dvars::ui_nativeCursor->current.enabled)
			Window::CursorVisible = TRUE;
		else
			Game::UI_DrawHandlePic(color, Game::scrPlace, x, y, w, h, horzAlign, vertAlign, material);
	}

	__declspec(naked) void Window::DrawCursorStub()
	{
		const static uint32_t jump_offset = 0x565453;
		__asm 
		{
			push	edx;
			push	ecx;
			call	Window::DrawCursor;
			add		esp, 8;
			jmp		jump_offset;
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

		GUI::Reset();

		return Window::MainWindow;
	}

	void Window::ApplyCursor()
	{
		bool isLoading = !FastFiles::Ready();
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
	}

	BOOL WINAPI Window::MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		auto menu_open = false;

		if (const auto cb = WndMessageCallbacks.find(Msg); cb != WndMessageCallbacks.end())
		{
			return cb->second(lParam, wParam);
		}

		if (Game::gui.initialized)
		{
			// handle input and mouse cursor for open menus
			for (auto menu = 0; menu < 1; menu++)
			{
				if (Game::gui.menus[menu].menustate)
				{
					if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
					{
						ImGui::GetIO().MouseDrawCursor = true;
						return TRUE;
					}

					menu_open = true;
				}
			}

			ImGui::GetIO().MouseDrawCursor = false;
		}

		// draw cursor (if no imgui menu opened) when cod4 inactive and hovering over it
		if (!menu_open)
		{
			if (Msg == WM_SETCURSOR)
			{
				Window::ApplyCursor();
				return TRUE;
			}
		}

		return Utils::Hook::Call<BOOL(__stdcall)(HWND, UINT, WPARAM, LPARAM)>(0x596810)(hWnd, Msg, wParam, lParam);
	}

	void Window::EnableDpiAwareness()
	{
		const Utils::Library user32{ "user32.dll" };
		user32.invokePascal<void>("SetProcessDpiAwarenessContext", DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
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

	void Window::ScrPlace_CalcSafeAreaOffsets(float* realViewableMin, float* realViewableMax, float viewportX, float viewportY, float viewportWidth, float viewportHeight, float aspect, float safeAreaRatioHorz, float safeAreaRatioVert, float* virtualViewableMin, float* virtualViewableMax, int width, int height)
	{
		double v15;
		double v16;
		double v17;
		double v18;
		double v19;
		double v20;
		float v21;
		float v22;
		float v23;
		float v24;
		float v25;
		float v26;
		float v27;
		float v28;
		float a10a;
		float a11c;
		float a11a;
		float a11b;
		float a12b;
		float a12c;
		float a12d;
		int a12a;

		a11c = (float)width;
		v21 = (1.0 - safeAreaRatioHorz) * 0.5 * a11c;
		a10a = (float)height;
		v23 = (1.0 - safeAreaRatioVert) * 0.5 * a10a;
		a12b = v21 + 0.5;
		a12c = floor(a12b);
		v22 = a12c;
		a12d = v23 + 0.5;
		*(float*)&a12a = floor(a12d);
		v27 = a11c - v22;
		v28 = a10a - *(float*)&a12a;
		if (viewportX >= (double)v22)
			v22 = viewportX;
		if (viewportY < (double)*(float*)&a12a)
		{
			v15 = viewportY;
			v16 = viewportX;
			v24 = *(float*)&a12a;
		}
		else
		{
			v15 = viewportY;
			v16 = viewportX;
			v24 = viewportY;
		}
		v17 = viewportWidth;
		a11a = v16 + viewportWidth;
		v18 = v27;
		if (a11a <= (double)v27)
			v18 = a11a;
		v25 = v18;
		v19 = viewportHeight;
		a11b = v15 + viewportHeight;
		v20 = v28;
		if (a11b <= (double)v28)
			v20 = a11b;
		v26 = v20;
		*realViewableMin = v22 - v16;
		realViewableMin[1] = v24 - v15;
		*realViewableMax = v25 - v16;
		realViewableMax[1] = v26 - v15;
		*virtualViewableMin = *realViewableMin * aspect * (640.0 / v17);
		virtualViewableMin[1] = realViewableMin[1] * (480.0 / v19);
		*virtualViewableMax = 640.0 / v17 * (aspect * *realViewableMax);
		virtualViewableMax[1] = 480.0 / v19 * realViewableMax[1];
	}

	void Window::ScrPlace_SetupViewport(Game::ScreenPlacement* scrPlace, float viewportX, float viewportY, float viewportWidth, float viewportHeight)
	{
		float vidConfig_aspectRatioScenePixel = *(float*)0x85BCBC;

		scrPlace->realViewportSize[0] = viewportWidth;
		scrPlace->realViewportSize[1] = viewportHeight;
		float adjustedRealWidth = 1.333333373069763 * viewportHeight / vidConfig_aspectRatioScenePixel;
		if (adjustedRealWidth > viewportWidth)
			adjustedRealWidth = viewportWidth;

		Window::ScrPlace_CalcSafeAreaOffsets(
			scrPlace->realViewableMin,
			scrPlace->realViewableMax,
			viewportX,
			viewportY,
			viewportWidth,
			viewportHeight,
			viewportWidth / adjustedRealWidth,
			Dvars::safeArea_horizontal->current.value,
			Dvars::safeArea_vertical->current.value,
			scrPlace->virtualViewableMin,
			scrPlace->virtualViewableMax,
			viewportWidth,
			viewportHeight
		);

		//Window::ScrPlace_CalcSafeAreaOffsets(
		//	scrPlace->realAdjustableMin,
		//	scrPlace->realAdjustableMax,
		//	viewportX,
		//	viewportY,
		//	viewportWidth,
		//	viewportHeight,
		//	viewportWidth / adjustedRealWidth,
		//	Dvars::Functions::Dvar_FindVar("safeArea_adjusted_horizontal")->current.value,
		//	Dvars::Functions::Dvar_FindVar("safeArea_adjusted_vertical")->current.value,
		//	scrPlace->virtualAdjustableMin,
		//	scrPlace->virtualAdjustableMax,
		//	viewportWidth,
		//	viewportHeight
		//);

		scrPlace->scaleVirtualToReal[0] = adjustedRealWidth / 640.0;
		scrPlace->scaleVirtualToReal[1] = viewportHeight / 480.0;
		scrPlace->scaleVirtualToFull[0] = viewportWidth / 640.0;
		scrPlace->scaleVirtualToFull[1] = viewportHeight / 480.0;
		scrPlace->scaleRealToVirtual[0] = 640.0 / adjustedRealWidth;
		scrPlace->scaleRealToVirtual[1] = 480.0 / viewportHeight;
		scrPlace->subScreenLeft = (viewportWidth - adjustedRealWidth) * 0.5;
	}

	Window::Window()
	{
		Events::OnDvarInit([]
		{
			Dvars::r_noborder = Dvars::Register::Dvar_RegisterBool("r_noborder", "Do not use a border in windowed mode", false, Game::saved);
			Dvars::ui_nativeCursor = Dvars::Register::Dvar_RegisterBool("ui_nativeCursor", "Display native cursor", false, Game::saved);

			Dvars::safeArea_horizontal = Dvars::Register::Dvar_RegisterFloat("safeArea_horizontal", "Horizontal safe area as a fraction of the screen width", 1.0f, 0.85f, 1.0f, Game::saved);
			Dvars::safeArea_vertical = Dvars::Register::Dvar_RegisterFloat("safeArea_vertical", "Vertical safe area as a fraction of the screen height", 1.0f, 0.85f, 1.0f, Game::saved);

			Game::dvar_s* ui_safearea = Dvars::Register::Dvar_RegisterBool("ui_safearea", "Shows the safe area", 0, Game::none);

			static std::vector <const char*> r_videomode_values =
			{
				"fullscreen",
				"windowed",
				"windowed_without_borders",
			};

			Dvars::r_videomode = Dvars::Register::Dvar_RegisterEnum("r_videomode", "Video mode", 0, r_videomode_values.size(), r_videomode_values.data(), Game::saved);
		});

		UIScript::Add("applyVideoMode", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			if (Dvars::r_videomode->current.integer == 0)
			{
				Command::Execute("set r_noborder 0");
				Command::Execute("set r_fullscreen 1");
			}
			else if (Dvars::r_videomode->current.integer == 1)
			{
				Command::Execute("set r_noborder 0");
				Command::Execute("set r_fullscreen 0");
			}
			else
			{
				Command::Execute("set r_fullscreen 0");
				Command::Execute("set r_noborder 1");
			}
		});

		Command::Add("resetViewport", [](Command::Params*)
		{
			int cls_rendererStarted = static_cast<int>(0x85BC04);

			if (cls_rendererStarted)
			{
				int vidConfig_displayHeight = *(int*)0x85BCA8;
				int vidConfig_displayWidth = *(int*)0x85BCA4;
				Window::ScrPlace_SetupViewport(Game::scrPlaceFullUnsafe, 0.0f, 0.0f, vidConfig_displayWidth, vidConfig_displayHeight);
				Window::ScrPlace_SetupViewport(Game::scrPlace, 0.0f, 0.0f, vidConfig_displayWidth, vidConfig_displayHeight);
				Window::ScrPlace_SetupViewport(Game::scrPlaceFull, 0.0f, 0.0f, vidConfig_displayWidth, vidConfig_displayHeight);
			}
		});

		// Main window border
		Utils::Hook(0x5D9623, Window::StyleHookStub, HOOK_JUMP).install()->quick();

		// Main window creation
		Utils::Hook::Nop(0x5D968A, 6);
		Utils::Hook(0x5D968A, Window::CreateMainWindow, HOOK_CALL).install()->quick();

		// Mark the cursor as visible
		Utils::Hook(0x56544E, Window::DrawCursorStub, HOOK_JUMP).install()->quick();

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

		// Don't let the game interact with the native cursor
		Utils::Hook::Set(0x65D2B8, Window::ShowCursorHook);

		// Use custom message handler
		Utils::Hook::Set(0x595C1E, Window::MessageHandler);

		Window::OnWndMessage(WM_SETCURSOR, [](WPARAM, LPARAM)
		{
			Window::ApplyCursor();
			return TRUE;
		});

		Window::EnableDpiAwareness();

		Utils::Hook::Nop(0x445427, 5);
		Utils::Hook(0x445427, Window::ScrPlace_SetupViewport, HOOK_CALL).install()->quick();
		Utils::Hook::Nop(0x445451, 5);
		Utils::Hook(0x445451, Window::ScrPlace_SetupViewport, HOOK_CALL).install()->quick();
		Utils::Hook::Nop(0x44547B, 5);
		Utils::Hook(0x44547B, Window::ScrPlace_SetupViewport, HOOK_CALL).install()->quick();

		// Do not use vid_xpos / vid_ypos when r_noborder is enabled
		Utils::Hook::Nop(0x5D990C, 9);
		Utils::Hook(0x5D990C, vid_xypos_stub, HOOK_JUMP).install()->quick();

		Utils::Hook(0x5D9BC9, ExternalConsoleStub, HOOK_CALL).install()->quick();
	}

	Window::~Window()
	{
	}
}