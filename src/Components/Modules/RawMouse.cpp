#include "STDInc.hpp"

namespace Components
{
	int RawMouse::MouseRawX = 0;
	int RawMouse::MouseRawY = 0;

	void RawMouse::IN_ShowSystemCursor(bool show)
	{
		int desiredShow;
		int actualShow;

		desiredShow = show - 1;
		for (actualShow = ShowCursor(show); actualShow != desiredShow; actualShow = ShowCursor(actualShow < desiredShow))
			;
	}

	int RawMouse::CL_MouseEvent(int x, int y, int dx, int dy)
	{
		//if ((Game::clientUIActives->keyCatchers & Game::KEYCATCH_UI) && !Gamepad::IsGamePadInUse())
		if (Game::Key_IsCatcherActive(Game::KEYCATCH_UI) && !Gamepad::IsGamePadInUse())
		{
			Game::UI_MouseEvent(x, y);
			return 0;
		}
		if (Game::Key_IsCatcherActive(Game::KEYCATCH_CONSOLE))
		{
			IN_ShowSystemCursor(Dvars::Functions::Dvar_FindVar("r_fullscreen")->current.enabled == 0);
			return 0;
		}
		if (Gamepad::IsGamePadInUse())
		{
			ShowCursor(false);
			return 0;
		}
		else
		{
			for (int i = ShowCursor(0); i != -1; i = ShowCursor(i < -1))
				;
			if (!Dvars::Functions::Dvar_FindVar("sv_running")->current.enabled || 
				Dvars::Functions::Dvar_FindVar("cl_paused")->current.integer != 1)
			{
				Game::clients->mouseDx[Game::clients->mouseIndex] += dx;
				Game::clients->mouseDy[Game::clients->mouseIndex] += dy;
			}
			return 1;
		}
	}

	void RawMouse::IN_ClampMouseMove()
	{
		tagRECT rc;
		tagPOINT curPos;

		GetCursorPos(&curPos);
		GetWindowRect(Window::GetWindow(), &rc);
		auto isClamped = false;
		if (curPos.x >= rc.left)
		{
			if (curPos.x >= rc.right)
			{
				curPos.x = rc.right - 1;
				isClamped = true;
			}
		}
		else
		{
			curPos.x = rc.left;
			isClamped = true;
		}
		if (curPos.y >= rc.top)
		{
			if (curPos.y >= rc.bottom)
			{
				curPos.y = rc.bottom - 1;
				isClamped = true;
			}
		}
		else
		{
			curPos.y = rc.top;
			isClamped = true;
		}

		if (isClamped)
		{
			SetCursorPos(curPos.x, curPos.y);
		}
	}

	BOOL RawMouse::OnRawInput(LPARAM lParam, WPARAM)
	{
		auto dwSize = sizeof(RAWINPUT);
		static BYTE lpb[sizeof(RAWINPUT)];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

		auto* raw = reinterpret_cast<RAWINPUT*>(lpb);
		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			// Is there's really absolute mouse on earth?
			if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
			{
				MouseRawX = raw->data.mouse.lLastX;
				MouseRawY = raw->data.mouse.lLastY;
			}
			else
			{
				MouseRawX += raw->data.mouse.lLastX;
				MouseRawY += raw->data.mouse.lLastY;
			}
		}

		return TRUE;
	}

	void RawMouse::IN_RawMouseMove()
	{
		if (GetForegroundWindow() == Window::GetWindow())
		{
			if (Dvars::Functions::Dvar_FindVar("r_fullscreen")->current.enabled)
				IN_ClampMouseMove();

			static auto oldX = 0, oldY = 0;

			auto dx = MouseRawX - oldX;
			auto dy = MouseRawY - oldY;

			oldX = MouseRawX;
			oldY = MouseRawY;

			// Don't use raw input for menu?
			// Because it needs to call the ScreenToClient
			tagPOINT curPos;
			GetCursorPos(&curPos);
			Game::s_wmv->oldPos = curPos;
			ScreenToClient(Window::GetWindow(), &curPos);

			Gamepad::OnMouseMove(curPos.x, curPos.y, dx, dy);
			auto recenterMouse = CL_MouseEvent(curPos.x, curPos.y, dx, dy);

			if (recenterMouse)
				Game::IN_RecenterMouse();
		}
	}

	void RawMouse::IN_RawMouse_Init()
	{
		if (Window::GetWindow() && Dvars::M_RawInput->current.enabled)
		{
			RAWINPUTDEVICE Rid[1];
			Rid[0].usUsagePage = 0x01; // HID_USAGE_PAGE_GENERIC
			Rid[0].usUsage = 0x02; // HID_USAGE_GENERIC_MOUSE
			Rid[0].dwFlags = RIDEV_INPUTSINK;
			Rid[0].hwndTarget = Window::GetWindow();

			RegisterRawInputDevices(Rid, ARRAYSIZE(Rid), sizeof(Rid[0]));
		}
	}

	void RawMouse::IN_Init()
	{
		Game::IN_Init();
		IN_RawMouse_Init();
	}

	void RawMouse::IN_MouseMove()
	{
		Dvars::M_RawInput->current.enabled ? IN_RawMouseMove() : Game::IN_MouseMove();
	}

	RawMouse::RawMouse()
	{
		Utils::Hook(0x5948F0, IN_MouseMove, HOOK_JUMP).install()->quick();
		Utils::Hook(0x594913, IN_MouseMove, HOOK_JUMP).install()->quick();

		Utils::Hook(0x595756, IN_Init, HOOK_CALL).install()->quick();
		Utils::Hook(0x5958FD, IN_Init, HOOK_CALL).install()->quick();

		Dvars::M_RawInput = Dvars::Register::Dvar_RegisterBool("m_rawinput", "Use raw mouse input, Improves accuracy & has better support for higher polling rates", true, Game::saved);
		Window::OnWndMessage(WM_INPUT, OnRawInput);
		Window::OnCreate(IN_RawMouse_Init);
	}

	RawMouse::~RawMouse()
	{
	}
}