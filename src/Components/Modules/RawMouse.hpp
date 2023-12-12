#pragma once

namespace Components
{
	class RawMouse : public Component
	{
	public:
		RawMouse();
		~RawMouse();

		static void IN_MouseMove();
		static int CL_MouseEvent(int x, int y, int dx, int dy); //for Gamepad.cpp
	private:
		static int MouseRawX, MouseRawY;
		static void IN_ShowSystemCursor(bool show);
		static void IN_ClampMouseMove();
		static BOOL OnRawInput(LPARAM lParam, WPARAM);
		static void IN_RawMouseMove();
		static void IN_RawMouse_Init();
		static void IN_Init();
	};
}