#pragma once

namespace Components
{
	class GUI : public Component
	{
	public:
		static void CursorRedraw();
		static void RegisterMenu(Game::gui_menus_t menu, const std::function<void()>& callback);
		static void LoopRender();
		static void SetMenuLayout(Game::gui_menus_t& menu, const float x, const float y, const float width, const float height, const int horzAlign, const int vertAlign);
		static void Reset();
		static void MouseReset();
		static void ToggleKey(Game::gui_menus_t& menu, int keycatcher, bool on_command);
		static void ToggleMouse(bool state);

		GUI();
		~GUI();
	private:
		static void Init();
		static void BeginFrame();
		static void EndFrame();
		static bool AnyOpenMenu();
	};
}