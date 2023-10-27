#include "STDInc.hpp"

namespace Components
{
	const auto changeLogFile = "https://master.iw3spmod.site/iw3sp_data/CHANGELOG.md";
	Changelog::Changelog_t Changelog::changeLogData;
	float TEXT_ALPHA;
	ImGuiWindowFlags changeLogWindowFlags;

	//new
	void Changelog::CreateChangeLog(Game::gui_menus_t& menu)
	{
		ImVec4 TEXT_COLOR_RED(1.0f, 0.0f, 0.0f, TEXT_ALPHA);
		ImVec4 TEXT_COLOR_WHITE(1.0f, 1.0f, 1.0f, TEXT_ALPHA);

		// do not keep mouse cursor around if another ImGui was active
		menu.mouse_ignores_menustate = true;

		ImGuiIO& io = ImGui::GetIO();
		io.WantCaptureKeyboard = false;
		io.WantCaptureMouse = true;
		int pushed_styles = 0, pushed_colors = 0;

		// set window size once
		if (!menu.one_time_init)
		{
			menu.got_layout_from_menu = false;
			for (auto m = 0; m < Game::uiInfo->uiDC.openMenuCount; m++)
			{
				if (Game::uiInfo->uiDC.menuStack[m] && !_stricmp(Game::uiInfo->uiDC.menuStack[m]->window.name, "changelog_menu"))
				{
					for (auto i = 0; i < Game::uiInfo->uiDC.menuStack[m]->itemCount; i++)
					{
						if (Game::uiInfo->uiDC.menuStack[m]->items[i] && Game::uiInfo->uiDC.menuStack[m]->items[i]->window.name)
						{
							if (!_stricmp(Game::uiInfo->uiDC.menuStack[m]->items[i]->window.name, "changelog_area"))
							{
								GUI::SetMenuLayout(
									menu,
									Game::uiInfo->uiDC.menuStack[m]->items[i]->window.rect.x,
									Game::uiInfo->uiDC.menuStack[m]->items[i]->window.rect.y,
									Game::uiInfo->uiDC.menuStack[m]->items[i]->window.rect.w,
									Game::uiInfo->uiDC.menuStack[m]->items[i]->window.rect.h,
									Game::uiInfo->uiDC.menuStack[m]->items[i]->window.rect.horzAlign,
									Game::uiInfo->uiDC.menuStack[m]->items[i]->window.rect.vertAlign);
								menu.got_layout_from_menu = true;
							}
						}
					}
				}
			}
		}

		if (menu.got_layout_from_menu)
		{
			if (!menu.one_time_init)
			{
				Game::ScrPlace_ApplyRect(Game::scrPlaceFull, &menu.position[0], &menu.position[1], &menu.size[0], &menu.size[1], menu.horzAlign, menu.vertAlign);
				ImGui::SetNextWindowPos(ImVec2(menu.position[0], menu.position[1]), ImGuiCond_Always); //ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowSize(ImVec2(menu.size[0], menu.size[1]), ImGuiCond_Always); //ImGuiCond_FirstUseEver);
				menu.one_time_init = true;
			}
		}
		else // disable changelog gui
		{
			if (pushed_styles)
			{
				ImGui::PopStyleVar(pushed_styles);	pushed_styles = 0;
			}

			menu.menustate = false;
			return;
		}

		//check again if user open the changelog menu
		for (auto m = 0; m < Game::uiInfo->uiDC.openMenuCount; m++)
		{
			if (Game::uiInfo->uiDC.menuStack[m] && !_stricmp(Game::uiInfo->uiDC.menuStack[m]->window.name, "changelog_menu"))
				menu.menustate = true;
			else
				menu.menustate = false;
		}

		if (menu.menustate)
		{
			// changelog styles
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);				pushed_styles++;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);				pushed_styles++;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));	pushed_styles++;

			// scollbar style
			ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(1, 1, 1, 0.025f));

			changeLogWindowFlags = 0; //clean up the flags

			if (Game::Key_IsCatcherActive(0, Game::KEYCATCH_CONSOLE))
			{
				char con_outputVisible = *(char*)0x8114C4;
				int matchCount = *(int*)0x800CC4;
				int inputTextLen = *(int*)0x800CCC;
				char activeApp = *(char*)0x13E3FE4;

				if ((matchCount >= 7 && matchCount <= 24  && inputTextLen > 0 ) || con_outputVisible && activeApp)
				{
					changeLogWindowFlags |= ImGuiWindowFlags_NoTitleBar |
						ImGuiWindowFlags_NoBackground |
						ImGuiWindowFlags_NoResize |
						ImGuiWindowFlags_NoMove |
						ImGuiWindowFlags_NoSavedSettings |
						ImGuiWindowFlags_NoScrollWithMouse |
						ImGuiWindowFlags_NoInputs |
						ImGuiWindowFlags_NoCollapse |
						ImGuiWindowFlags_NoBringToFrontOnFocus;

					TEXT_ALPHA = 0.1f;

					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(200, 229, 120, 25.5))); pushed_colors++;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(182, 209, 107, 25.5)));		 pushed_colors++;
				}
				if ((matchCount >= 7 && matchCount <= 24 && inputTextLen > 0) || con_outputVisible || !activeApp)
				{
					changeLogWindowFlags |= ImGuiWindowFlags_NoTitleBar |
						ImGuiWindowFlags_NoBackground |
						ImGuiWindowFlags_NoResize |
						ImGuiWindowFlags_NoMove |
						ImGuiWindowFlags_NoSavedSettings |
						ImGuiWindowFlags_NoScrollWithMouse |
						ImGuiWindowFlags_NoInputs |
						ImGuiWindowFlags_NoCollapse |
						ImGuiWindowFlags_NoBringToFrontOnFocus;

					TEXT_ALPHA = 0.1f;

					menu.one_time_init = false;

					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(200, 229, 120, 25.5))); pushed_colors++;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(182, 209, 107, 25.5)));		 pushed_colors++;
				}
				else
				{
					changeLogWindowFlags |= ImGuiWindowFlags_NoTitleBar |
						ImGuiWindowFlags_NoBackground |
						ImGuiWindowFlags_NoResize |
						ImGuiWindowFlags_NoMove |
						ImGuiWindowFlags_NoSavedSettings |
						ImGuiWindowFlags_NoCollapse |
						ImGuiWindowFlags_NoBringToFrontOnFocus;

					TEXT_ALPHA = 1.0f;
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(200, 229, 120, 203)));  pushed_colors++;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(182, 209, 107, 100)));		 pushed_colors++;
				}
			}
			else
			{
				changeLogWindowFlags |= ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoBackground |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoBringToFrontOnFocus;

				TEXT_ALPHA = 1.0f;

				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(200, 229, 120, 203)));	pushed_colors++;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(182, 209, 107, 100)));		pushed_colors++;
			}

			// early out if the window is collapsed, as an optimization.
			if (!ImGui::Begin("Changelog", &menu.menustate, changeLogWindowFlags))
			{
				// pop scrollbar style
				ImGui::PopStyleColor(1);

				// pop all styles
				ImGui::PopStyleVar(pushed_styles);	pushed_styles = 0;

				ImGui::End();
				return;
			}

			// pop scrollbar style
			ImGui::PopStyleColor(1);

			// pop styles but keep the first one (alpha)
			ImGui::PopStyleVar(pushed_styles - 1);	pushed_styles = 1;

			// *
			// markdown
			ImGui::PushStyleColor(ImGuiCol_Text, changeLogData.status == CHANGELOG_ERROR ? TEXT_COLOR_RED : TEXT_COLOR_WHITE);
			pushed_colors++;

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);				pushed_styles++;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);				pushed_styles++;
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 8.0f);					pushed_styles++;

			ImGui::Indent(16.0f); ImGui::Dummy(ImVec2(0.0f, 4.0f));
			Markdown::MarkdownInit(changeLogData.changeLogText);
			ImGui::Indent(-16.0f); ImGui::Dummy(ImVec2(0.0f, 4.0f));

			// pop all colors/styles
			ImGui::PopStyleColor(pushed_colors);	pushed_colors = 0;
			ImGui::PopStyleVar(pushed_styles);		pushed_styles = 0;

			ImGui::End();

			// fullscreen gui
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

			io.WantCaptureKeyboard = false;
			io.WantCaptureMouse = false;
			
			
			if (!ImGui::Begin("", &menu.menustate,
				ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoBackground))
			{
				ImGui::End();
				return;
			}
			

			GUI::MouseReset();
			ImGui::End();
		}
	}

	Changelog::Changelog()
	{
		//check every 5 seconds changeLogData
		Scheduler::Loop([]
		{
			if (strlen(Dvars::Functions::Dvar_FindVar("fs_game")->current.string) == 0 && !Game::CL_IsCgameInitialized())
			{
				const auto changeLog = Utils::HTTP::GetData(changeLogFile, {}, {}, false);
				if (!changeLog.has_value())
				{
					changeLogData.status = CHANGELOG_ERROR;
					changeLogData.changeLogText = "Unable to get changelog.";
				}
				else
				{
					changeLogData.status = CHANGELOG_SUCCESS;
					changeLogData.changeLogText = changeLog.value().data();
				}
			}
		}, Scheduler::Pipeline::ASYNC, 5000ms);
	}

	Changelog::~Changelog()
	{
	}
}