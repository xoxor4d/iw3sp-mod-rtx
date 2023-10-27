#include "STDInc.hpp"

namespace Components
{
	void GUI::Init()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.IniFilename = NULL;

		io.Fonts->AddFontFromMemoryCompressedTTF(ImGui_Fonts::default_compressed_data, ImGui_Fonts::default_compressed_size, 18.0f);
		ImGui_Fonts::H2 = io.Fonts->AddFontFromMemoryCompressedTTF(ImGui_Fonts::defaultbold_compressed_data, ImGui_Fonts::defaultbold_compressed_size, 11.5f);
		ImGui_Fonts::H3 = io.Fonts->AddFontFromMemoryCompressedTTF(ImGui_Fonts::defaultbold_compressed_data, ImGui_Fonts::defaultbold_compressed_size, 10.0f);
		ImGui_Fonts::H1 = io.Fonts->AddFontFromMemoryCompressedTTF(ImGui_Fonts::defaultbold_compressed_data, ImGui_Fonts::defaultbold_compressed_size, 13.2f);

		ImGui_ImplWin32_Init(Window::GetWindow());
		ImGui_ImplDX9_Init(*Game::dx9_device_ptr);
		// Style
		ImGui::StyleColorsDark();
		Game::gui.initialized = true;
	}

	void GUI::SetMenuLayout(Game::gui_menus_t& menu, const float x, const float y, const float width, const float height, const int horzAlign = 4, const int vertAlign = 4)
	{
		menu.position[0] = x;
		menu.position[1] = y;

		menu.size[0] = width;
		menu.size[1] = height;

		menu.horzAlign = horzAlign;
		menu.vertAlign = vertAlign;
	}

	void GUI::RegisterMenu(Game::gui_menus_t menu, const std::function<void()>& callback)
	{
		if (menu.menustate) 
		{
			callback;
			menu.was_open = true;
		}
		else if (menu.was_open)
		{
			menu.was_open = false;
		}
	}

	void GUI::Reset()
	{
		if (Game::gui.initialized)
		{
			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();
			MouseReset();
			memset(&Game::gui, 0, sizeof(Game::gui_t));
		}
	}

	void GUI::CursorRedraw()
	{
		// get material handle
		Game::Material* cursor = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MATERIAL, "ui_cursor").material;

		float cur_w = (32.0f * Game::scrPlace->scaleVirtualToReal[0]) / Game::scrPlace->scaleVirtualToFull[0];
		float cur_h = (32.0f * Game::scrPlace->scaleVirtualToReal[1]) / Game::scrPlace->scaleVirtualToFull[1];
		float cur_x = Game::uiInfo->uiDC.cursor.x - 0.5f * cur_w;
		float cur_y = Game::uiInfo->uiDC.cursor.y - 0.5f * cur_h;

		float s0, s1;
		float t0, t1;

		if (cur_w >= 0.0f)
		{
			s0 = 0.0f;
			s1 = 1.0f;
		}
		else
		{
			cur_w = -cur_w;
			s0 = 1.0f;
			s1 = 0.0f;
		}

		if (cur_h >= 0.0f)
		{
			t0 = 0.0f;
			t1 = 1.0f;
		}
		else
		{
			cur_h = -cur_h;
			t0 = 1.0f;
			t1 = 0.0f;
		}

		// scale 640x480 rect to viewport resolution and draw the cursor
		Game::ScrPlace_ApplyRect(Game::scrPlace, &cur_x, & cur_w, & cur_y, & cur_h, 4, 4);
		Game::R_AddCmdDrawStretchPic(cursor, cur_x, cur_y, cur_w, cur_h, s0, t0, s1, t1, nullptr);
	}

	void GUI::BeginFrame()
	{
		//ImGui::GetIO().MouseDrawCursor = Game::gui.any_menus_open;
		//if (Game::gui.any_menus_open)
		//{
		//	Game::clientUIActives->keyCatchers |= 0x10;
		//}
		//else
		//{
		//	Game::clientUIActives->keyCatchers &= 0x10;
		//}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void GUI::EndFrame()
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	// check if there are any open menus
	bool GUI::AnyOpenMenu()
	{
		auto& ggui = Game::gui;
		const auto& cl_ingame = Dvars::Functions::Dvar_FindVar("cl_ingame");

		for (int m = 0; m < 1; m++)
		{
			if (ggui.menus[m].menustate)
			{
				// positive flag and ingame
				if (!ggui.any_menus_open && cl_ingame && cl_ingame->current.enabled)
				{
					// activate a dummy menu to block game input
					//CMDEXEC("menu_open_ingame pregame_loaderror_mp");
				}

				if (ggui.menus[m].mouse_ignores_menustate)
				{
					ggui.any_menus_open = false;
					return false;
				}

				ggui.any_menus_open = true;
				return true;
			}
		}

		// negative flag
		if (ggui.any_menus_open)
		{
			// close the dummy menu
			if (cl_ingame && cl_ingame->current.enabled)
			{
				//CMDEXEC("menu_closebyname pregame_loaderror_mp");
			}

			ggui.any_menus_open = false;
		}


		return false;
	}

	void GUI::LoopRender()
	{
		auto& ggui = Game::gui;

		if (!ggui.initialized)
		{
			Init();
		}

		BeginFrame();
		AnyOpenMenu();

		// ------------
		Changelog::CreateChangeLog(ggui.menus[Game::GUI_MENUS::CHANGELOG]);
		// ------------

		EndFrame();
	}

	void GUI::MouseReset()
	{
		if (const auto& in_mouse = Dvars::Functions::Dvar_FindVar("in_mouse"); in_mouse)
		{
			in_mouse->current.enabled = true;
			in_mouse->latched.enabled = true;
			Game::s_wmv->mouseInitialized = in_mouse->current.enabled;
		}
	}

	void GUI::ToggleMouse(bool state)
	{
		if (const auto& in_mouse = Dvars::Functions::Dvar_FindVar("in_mouse"); in_mouse)
		{
			if (state)
			{
				in_mouse->current.enabled = true;
				in_mouse->latched.enabled = true;
			}
			else
			{
				in_mouse->current.enabled = false;
				in_mouse->latched.enabled = true;
			}

			Utils::function<void()>(0x594610)(); // In_Shutdown
			Game::s_wmv->mouseInitialized = in_mouse->current.enabled;
		}
	}


	void GUI::ToggleKey(Game::gui_menus_t& menu, int keycatcher, bool on_command = false)
	{
		if (!Game::gui.initialized)
		{
			return;
		}

		if (Game::playerKeys->keys[keycatcher].down)
		{
			menu.hk_is_clicked = false;
			menu.hk_is_down = true;
		}
		else if (!Game::playerKeys->keys[keycatcher].down && menu.hk_is_down)
		{
			menu.hk_is_clicked = true;
			menu.hk_is_down = false;
		}
		else
		{
			menu.hk_is_clicked = false;
			menu.hk_is_down = false;
		}

		// toggle menu by key or command
		if (menu.hk_is_clicked || on_command)
		{
			ImGuiIO& io = ImGui::GetIO();
			std::fill_n(io.KeysDown, 512, 0); // fix keys getting stuck on close / vid_restart

			menu.menustate = !menu.menustate;

			// on close
			if (!menu.menustate)
			{
				// check if there is still some open menu
				if (Game::gui.any_menus_open)
				{
					// do nothing with the mouse
					return;
				}
			}

			// toggle the mouse
			GUI::ToggleMouse(menu.menustate);
		}
	}

	GUI::GUI()
	{
		Command::Add("check_gui_state_menu", [](Command::Params*)
		{
			if (Game::gui.menus[Game::GUI_MENUS::CHANGELOG].menustate)
				Game::Com_Printf(0, "menu enabled\n");
			else
				Game::Com_Printf(0, "menu disabled\n");
		});
		Scheduler::Loop([]
		{
		}, Scheduler::Pipeline::MAIN);
	}

	GUI::~GUI()
	{
	}
}