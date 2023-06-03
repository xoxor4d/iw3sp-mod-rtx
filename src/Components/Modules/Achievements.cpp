#include "STDInc.hpp"

// Achivements module by fedddddd adapted for IW3SP.
//https://github.com/fedddddd/h2-mod/blob/develop/src/client/component/achievements.cpp
namespace Components
{
	std::mutex file_mutex;

	std::array<Achievements::achievement_t, Achievements::ACHIEVEMENT_TOTAL_COUNT> Achievements::achievements =
	{
		achievement_t(ACHIEVEMENT_ALL_ACHIEVEMENTS, "ACHIEVEMENTS_COMPLETED"),
		achievement_t(ACHIEVEMENT_1, "EARN_A_WINGED_DAGGER"),
		achievement_t(ACHIEVEMENT_2, "MAKE_THE_JUMP"),
		achievement_t(ACHIEVEMENT_3, "COMPLETE_BLACKOUT"),
		achievement_t(ACHIEVEMENT_4, "SAVE_THE_BACON"),
		achievement_t(ACHIEVEMENT_5, "BRING_EM_HOME"),
		achievement_t(ACHIEVEMENT_6, "COMPLETE_VILLAGE_ASSAULT"),
		achievement_t(ACHIEVEMENT_7, "PIGGYBACK_RIDE"),
		achievement_t(ACHIEVEMENT_8, "DESPERATE_MEASURES"),
		achievement_t(ACHIEVEMENT_9, "WIN_THE_WAR"),
		achievement_t(ACHIEVEMENT_10, "LOOK_SHARP"),
		achievement_t(ACHIEVEMENT_11, "EARS_AND_EYES"),
		achievement_t(ACHIEVEMENT_12, "DOWN_BOY_DOWN"),
		achievement_t(ACHIEVEMENT_13, "NEW_SQUADRON_RECORD"),
		achievement_t(ACHIEVEMENT_14, "RESCUE_ROYCEWICZ"),
		achievement_t(ACHIEVEMENT_15, "YOUR_SHOW_SUCKS"),
		achievement_t(ACHIEVEMENT_16, "MAN_OF_THE_PEOPLE"),
		achievement_t(ACHIEVEMENT_17, "STRAIGHT_FLUSH"),
		achievement_t(ACHIEVEMENT_18, "GHILLIES_IN_THE_MIST"),
		achievement_t(ACHIEVEMENT_19, "MILE_HIGH_CLUB"),
		achievement_t(ACHIEVEMENT_20, "NO_REST_FOR_THE_WEARY"),
		achievement_t(ACHIEVEMENT_21, "WAR_HARDENED"),
		achievement_t(ACHIEVEMENT_22, "THE_PACKAGE"),
		achievement_t(ACHIEVEMENT_23, "THE_RESCUE"),
		achievement_t(ACHIEVEMENT_24, "THE_SEARCH"),
		achievement_t(ACHIEVEMENT_25, "THE_BOG"),
		achievement_t(ACHIEVEMENT_26, "THE_ESCAPE"),
		achievement_t(ACHIEVEMENT_27, "THE_FIRST_HORSEMAN"),
		achievement_t(ACHIEVEMENT_28, "THE_SECOND_HORSEMAN"),
		achievement_t(ACHIEVEMENT_29, "THE_SHOT"),
		achievement_t(ACHIEVEMENT_30, "THE_THIRD_HORSEMAN"),
		achievement_t(ACHIEVEMENT_31, "THE_ULTIMATUM"),
		achievement_t(ACHIEVEMENT_32, "THE_FOURTH_HORSEMAN"),
		achievement_t(ACHIEVEMENT_33, "DAREDEVIL"),
		achievement_t(ACHIEVEMENT_34, "ROADKILL"),
		achievement_t(ACHIEVEMENT_35, "BIRD_ON_THE_GROUND"),
		achievement_t(ACHIEVEMENT_36, "FOUR_OF_A_KIND"),
		achievement_t(ACHIEVEMENT_37, "THREE_OF_A_KIND"),
	};

	bool Achievements::HasAchievement(achievement_file_t* file, int id)
	{
		if (id >= ACHIEVEMENT_TOTAL_COUNT)
		{
			return false;
		}

		return file->achievements[id];
	}

	bool Achievements::HasAchievement(int id)
	{
		achievement_file_t file{};
		GetAchievementsData(&file);
		return HasAchievement(&file, id);
	}

	std::string Achievements::GetAchievementName(int id) { return Utils::String::VA("ACHIEVEMENT_NAME_%i", id); }
	std::string Achievements::GetAchievementDetails(int id) { return Utils::String::VA("ACHIEVEMENT_DETAIL_%i", id); }

	std::filesystem::path AchievementPath()
	{
		auto fileBuffer = Utils::IO::ReadFile("players/profiles/active.txt");
		if (fileBuffer.empty())
			return "";

		std::filesystem::path game_folder = std::format("{}\\players\\profiles\\{}\\achievements.bin", Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string, fileBuffer.c_str());
		return game_folder;
	}

	std::optional<int> Achievements::GetAchievementID(const std::string& name)
	{
		for (auto i = 0; i < ACHIEVEMENT_TOTAL_COUNT; i++)
		{
			const auto achievement = &achievements[i];
			if (name == achievement->code)
			{
				return { achievement->id };
			}
		}

		return {};
	}

	void Achievements::WriteAchievement(achievement_file_t* data)
	{
		std::lock_guard _0(file_mutex);

		data->version = ACHIEVEMENT_FILE_VERSION;
		data->signature = ACHIEVEMENT_FILE_SIGNATURE;

		const auto path = AchievementPath();
		const auto str = std::string(reinterpret_cast<char*>(data), sizeof(achievement_file_t));
		Utils::IO::WriteFile(path.generic_string(), str, false);
	}

	bool Achievements::CheckThePlatinum(achievement_file_t* file)
	{
		if (HasAchievement(file, ACHIEVEMENT_ALL_ACHIEVEMENTS)) return false;

		//from 1 to 37
		for (auto i = static_cast<int>(ACHIEVEMENT_START); i < ACHIEVEMENT_TOTAL_COUNT; i++)
		{
			if (!file->achievements[i])
				return false;
		}

		return true;
	}

	void Achievements::GiveAchievementByID(achievement_file_t* file, int id)
	{
		GiveAchievementByID_Internal(file, id);
		if (CheckThePlatinum(file))
		{
			GiveAchievementByID_Internal(file, ACHIEVEMENT_ALL_ACHIEVEMENTS);
		}
	}

	void Achievements::GiveAchievement(const std::string& name)
	{
		Achievements::achievement_file_t file{};

		const auto id = GetAchievementID(name);
		if (!id.has_value())
		{
			return;
		}

		GiveAchievementByID(&file, id.value());
	}

	void Achievements::GetAchievementsData(achievement_file_t* file)
	{
		std::lock_guard _0(file_mutex);
		const auto path = AchievementPath().generic_string();

		if (!Utils::IO::FileExists(path)) return;

		const auto data = Utils::IO::ReadFile(path);
		if (data.size() < sizeof(achievement_file_t)) return;

		std::memcpy(file, data.data(), std::min(data.size(), sizeof(achievement_file_t)));
		if (file->signature != ACHIEVEMENT_FILE_SIGNATURE)
		{
			std::memset(file, 0, sizeof(achievement_file_t));
		}
	}

	void Achievements::GiveAchievementByID_Internal(achievement_file_t* file, int id)
	{
		GetAchievementsData(file);
		if (HasAchievement(file, id))
		{
			Game::Com_Printf(0, "^1Achievement Debug: you already unlock this achievement\n");
			return;
		}

		file->achievements[id] = true;
		file->timeStamp[id] = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		WriteAchievement(file);

		Game::Material* material = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MATERIAL, Utils::String::VA("trophy_%d", id)).material;
		Toast::Show(material, Game::UI_SafeTranslateString("ACHIEVEMENT_EARNED"), Game::UI_SafeTranslateString(GetAchievementName(id).c_str()), 4000, true);

		// Details may be too very long for the form.
		//Toast::Show(material, Game::UI_SafeTranslateString(get_name(id).c_str()), Game::UI_SafeTranslateString(get_details(id).c_str()), 4000, true);
	}

	int Achievements::GetEarnedAchievementCount(achievement_file_t* file)
	{
		int count = 0;

		for (bool achievement : file->achievements)
		{
			if (achievement)
				++count;
		}

		return count;
	}

	int CalculateProgressBarWidth(int totalWidth, int playerAchievements, int totalAchievements) {
		double progress = static_cast<double>(playerAchievements) / totalAchievements;
		int progressBarWidth = static_cast<int>(progress * totalWidth + 0.5);
		return progressBarWidth;
	}

	const char* VA_fake(const char* fmt, ...)
	{
		static char g_vaBuffer[64][65536];
		static int g_vaNextBufferIndex = 0;

		va_list ap;
		va_start(ap, fmt);
		char* dest = g_vaBuffer[g_vaNextBufferIndex];
		vsnprintf(g_vaBuffer[g_vaNextBufferIndex], 65536, fmt, ap);
		g_vaNextBufferIndex = (g_vaNextBufferIndex + 1) % 64;
		va_end(ap);
		return dest;
	}

	Achievements::Achievements()
	{
		Events::OnDvarInit([]
		{
			// I'm use the dvars because in current moment UIScript token doesn't work.
			Dvars::Register::Dvar_RegisterInt("ui_achievement_status", "", 0, 0, 1, Game::none);
			Dvars::Register::Dvar_RegisterInt("ui_achievement_select_idx", "", 0, 0, 37, Game::read_only);
			Dvars::Register::Dvar_RegisterInt("ui_achievement_page", "", 0, 0, 2, Game::none);
		});

		Command::Add("get_menu_text", [](Command::Params*)
		{
			Game::menuDef_t* menu1 = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements").menu;
			Game::menuDef_t* menu2 = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_1").menu;
			Game::menuDef_t* menu3 = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_2").menu;

			for (int i = 0; i < menu1->itemCount; i++)
			{
				Game::Com_Printf(0, "%d %s\n", i, menu1->items[i]->text);
			}
			Game::Com_Printf(0, "-------------------------------\n");
			for (int i = 0; i < menu2->itemCount; i++)
			{
				Game::Com_Printf(0, "%d %s\n", i, menu2->items[i]->text);
			}
			Game::Com_Printf(0, "-------------------------------\n");
			for (int i = 0; i < menu3->itemCount; i++)
			{
				Game::Com_Printf(0, "%d %s\n", i, menu3->items[i]->text);
			}
		});

		Command::Add("get_menu_element", [](Command::Params*)
		{
			//Game::menuDef_t* menu1 = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_2").menu;
			//Achievements::achievement_file_t file{};
			//GetAchievementsData(&file);

			//Game::Com_Printf(0, Utils::String::VA("Received: %d | Total: %d \n",
			//	Achievements::GetEarnedAchievementCount(&file), 
			//	ACHIEVEMENT_TOTAL_COUNT));

			//menu1->items[169]->window.rect.w = 0.000000;

			//for (int i = 0; i < menu1->itemCount; i++)
			//{
			//	Game::Com_Printf(0, Utils::String::VA("^1%d %s %f %f %f %f\n",
			//		i,
			//		menu1->items[i]->window.name,
			//		menu1->items[i]->window.rect.x,
			//		menu1->items[i]->window.rect.y,
			//		menu1->items[i]->window.rect.w,
			//		menu1->items[i]->window.rect.h));
			//}
			//menu1->items[170]->window.rectClient.w = 250.000000;
		});

		Command::Add("get_menu_text_ingame", [](Command::Params*)
			{
				Game::menuDef_t* menu1 = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_ingame").menu;
				Game::menuDef_t* menu2 = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_1_ingame").menu;
				Game::menuDef_t* menu3 = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_2_ingame").menu;

				for (int i = 0; i < menu1->itemCount; i++)
				{
					Game::Com_Printf(0, "%d %s\n", i, menu1->items[i]->text);
				}
				Game::Com_Printf(0, "-------------------------------\n");
				for (int i = 0; i < menu2->itemCount; i++)
				{
					Game::Com_Printf(0, "%d %s\n", i, menu2->items[i]->text);
				}
				Game::Com_Printf(0, "-------------------------------\n");
				for (int i = 0; i < menu3->itemCount; i++)
				{
					Game::Com_Printf(0, "%d %s\n", i, menu3->items[i]->text);
				}
			});

		//Sadly, but in current moment, I can't get the args from token
		UIScript::Add("achievement_get_info", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			Achievements::achievement_file_t file{};
			GetAchievementsData(&file);


			if (!Game::CL_IsCgameInitialized())
			{
				Game::menuDef_t* achievement_menu_1_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements").menu;
				Game::menuDef_t* achievement_menu_2_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_1").menu;
				Game::menuDef_t* achievement_menu_3_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_2").menu;

				int achievement_index_select = Dvars::Functions::Dvar_FindVar("ui_achievement_select_idx")->current.integer;


				if (HasAchievement(&file, Dvars::Functions::Dvar_FindVar("ui_achievement_select_idx")->current.integer))
				{
					Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer = 1;
					Dvars::Functions::Dvar_FindVar("ui_achievement_status")->latched.integer = 1;
				}
				else
				{
					Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer = 0;
					Dvars::Functions::Dvar_FindVar("ui_achievement_status")->latched.integer = 0;
				}


				//First achievement menu
				if (Dvars::Functions::Dvar_FindVar("ui_achievement_page")->current.integer == 0)
				{
					//Check the achievement status. Changes the itemDefs if achievement has been received
					if (Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer == 1)
					{
						std::string timeString = std::ctime(&file.timeStamp[achievement_index_select]);
						timeString.pop_back();
						achievement_menu_1_page->items[25]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[35]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[45]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[55]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[65]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[75]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[85]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[95]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[105]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[115]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[125]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[135]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[145]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[155]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[165]->text = Utils::String::VA("%s", timeString.c_str());
					}
					else
					{
						achievement_menu_1_page->items[25]->text = "--:--";
						achievement_menu_1_page->items[35]->text = "--:--";
						achievement_menu_1_page->items[45]->text = "--:--";
						achievement_menu_1_page->items[55]->text = "--:--";
						achievement_menu_1_page->items[65]->text = "--:--";
						achievement_menu_1_page->items[75]->text = "--:--";
						achievement_menu_1_page->items[85]->text = "--:--";
						achievement_menu_1_page->items[95]->text = "--:--";
						achievement_menu_1_page->items[105]->text = "--:--";
						achievement_menu_1_page->items[115]->text = "--:--";
						achievement_menu_1_page->items[125]->text = "--:--";
						achievement_menu_1_page->items[135]->text = "--:--";
						achievement_menu_1_page->items[145]->text = "--:--";
						achievement_menu_1_page->items[155]->text = "--:--";
						achievement_menu_1_page->items[165]->text = "--:--";
					}
				}
				else if (Dvars::Functions::Dvar_FindVar("ui_achievement_page")->current.integer == 1)
				{
					//Check the achievement status. Changes the itemDefs if achievement has been received
					if (Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer == 1)
					{
						std::string timeString = std::ctime(&file.timeStamp[achievement_index_select]);
						timeString.pop_back();
						achievement_menu_2_page->items[25]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[35]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[45]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[55]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[65]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[75]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[85]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[95]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[105]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[115]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[125]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[135]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[145]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[155]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[165]->text = Utils::String::VA("%s", timeString.c_str());
					}
					else
					{
						achievement_menu_2_page->items[25]->text = "--:--";
						achievement_menu_2_page->items[35]->text = "--:--";
						achievement_menu_2_page->items[45]->text = "--:--";
						achievement_menu_2_page->items[55]->text = "--:--";
						achievement_menu_2_page->items[65]->text = "--:--";
						achievement_menu_2_page->items[75]->text = "--:--";
						achievement_menu_2_page->items[85]->text = "--:--";
						achievement_menu_2_page->items[95]->text = "--:--";
						achievement_menu_2_page->items[105]->text = "--:--";
						achievement_menu_2_page->items[115]->text = "--:--";
						achievement_menu_2_page->items[125]->text = "--:--";
						achievement_menu_2_page->items[135]->text = "--:--";
						achievement_menu_2_page->items[145]->text = "--:--";
						achievement_menu_2_page->items[155]->text = "--:--";
						achievement_menu_2_page->items[165]->text = "--:--";
					}
				}
				else
				{
					//Check the achievement status. Changes the itemDefs if achievement has been received
					if (Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer == 1)
					{
						std::string timeString = std::ctime(&file.timeStamp[achievement_index_select]);
						timeString.pop_back();
						achievement_menu_3_page->items[25]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[35]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[45]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[55]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[65]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[75]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[85]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[95]->text = Utils::String::VA("%s", timeString.c_str());
					}
					else
					{
						achievement_menu_3_page->items[25]->text = "--:--";
						achievement_menu_3_page->items[35]->text = "--:--";
						achievement_menu_3_page->items[45]->text = "--:--";
						achievement_menu_3_page->items[55]->text = "--:--";
						achievement_menu_3_page->items[65]->text = "--:--";
						achievement_menu_3_page->items[75]->text = "--:--";
						achievement_menu_3_page->items[85]->text = "--:--";
						achievement_menu_3_page->items[95]->text = "--:--";
					}
				}
			}
			else
			{
				Game::menuDef_t* achievement_menu_1_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_ingame").menu;
				Game::menuDef_t* achievement_menu_2_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_1_ingame").menu;
				Game::menuDef_t* achievement_menu_3_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_2_ingame").menu;

				int achievement_index_select = Dvars::Functions::Dvar_FindVar("ui_achievement_select_idx")->current.integer;

				if (HasAchievement(&file, Dvars::Functions::Dvar_FindVar("ui_achievement_select_idx")->current.integer))
				{
					Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer = 1;
					Dvars::Functions::Dvar_FindVar("ui_achievement_status")->latched.integer = 1;
				}
				else
				{
					Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer = 0;
					Dvars::Functions::Dvar_FindVar("ui_achievement_status")->latched.integer = 0;
				}

				if (Dvars::Functions::Dvar_FindVar("ui_achievement_page")->current.integer == 0)
				{
					//Check the achievement status. Changes the itemDefs if achievement has been received
					if (Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer == 1)
					{
						std::string timeString = std::ctime(&file.timeStamp[achievement_index_select]);
						timeString.pop_back();
						achievement_menu_1_page->items[19]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[29]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[39]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[49]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[59]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[69]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[79]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[89]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[99]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[109]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[119]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[129]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[139]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[149]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_1_page->items[159]->text = Utils::String::VA("%s", timeString.c_str());
					}
					else
					{
						achievement_menu_1_page->items[19]->text = "--:--";
						achievement_menu_1_page->items[29]->text = "--:--";
						achievement_menu_1_page->items[39]->text = "--:--";
						achievement_menu_1_page->items[49]->text = "--:--";
						achievement_menu_1_page->items[59]->text = "--:--";
						achievement_menu_1_page->items[69]->text = "--:--";
						achievement_menu_1_page->items[79]->text = "--:--";
						achievement_menu_1_page->items[89]->text = "--:--";
						achievement_menu_1_page->items[99]->text = "--:--";
						achievement_menu_1_page->items[109]->text = "--:--";
						achievement_menu_1_page->items[119]->text = "--:--";
						achievement_menu_1_page->items[129]->text = "--:--";
						achievement_menu_1_page->items[139]->text = "--:--";
						achievement_menu_1_page->items[149]->text = "--:--";
						achievement_menu_1_page->items[159]->text = "--:--";
					}
				}
				else if (Dvars::Functions::Dvar_FindVar("ui_achievement_page")->current.integer == 1)
				{
					//Check the achievement status. Changes the itemDefs if achievement has been received
					if (Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer == 1)
					{
						std::string timeString = std::ctime(&file.timeStamp[achievement_index_select]);
						timeString.pop_back();
						achievement_menu_2_page->items[19]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[29]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[39]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[49]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[59]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[69]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[79]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[89]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[99]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[109]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[119]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[129]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[139]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[149]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_2_page->items[159]->text = Utils::String::VA("%s", timeString.c_str());
					}
					else
					{
						achievement_menu_2_page->items[19]->text = "--:--";
						achievement_menu_2_page->items[29]->text = "--:--";
						achievement_menu_2_page->items[39]->text = "--:--";
						achievement_menu_2_page->items[49]->text = "--:--";
						achievement_menu_2_page->items[59]->text = "--:--";
						achievement_menu_2_page->items[69]->text = "--:--";
						achievement_menu_2_page->items[79]->text = "--:--";
						achievement_menu_2_page->items[89]->text = "--:--";
						achievement_menu_2_page->items[99]->text = "--:--";
						achievement_menu_2_page->items[109]->text = "--:--";
						achievement_menu_2_page->items[119]->text = "--:--";
						achievement_menu_2_page->items[129]->text = "--:--";
						achievement_menu_2_page->items[139]->text = "--:--";
						achievement_menu_2_page->items[149]->text = "--:--";
						achievement_menu_2_page->items[159]->text = "--:--";
					}
				}
				else
				{
					//Check the achievement status. Changes the itemDefs if achievement has been received
					if (Dvars::Functions::Dvar_FindVar("ui_achievement_status")->current.integer == 1)
					{
						std::string timeString = std::ctime(&file.timeStamp[achievement_index_select]);
						timeString.pop_back();
						achievement_menu_3_page->items[19]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[29]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[39]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[49]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[59]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[69]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[79]->text = Utils::String::VA("%s", timeString.c_str());
						achievement_menu_3_page->items[89]->text = Utils::String::VA("%s", timeString.c_str());
					}
					else
					{
						achievement_menu_3_page->items[19]->text = "--:--";
						achievement_menu_3_page->items[29]->text = "--:--";
						achievement_menu_3_page->items[39]->text = "--:--";
						achievement_menu_3_page->items[49]->text = "--:--";
						achievement_menu_3_page->items[59]->text = "--:--";
						achievement_menu_3_page->items[69]->text = "--:--";
						achievement_menu_3_page->items[79]->text = "--:--";
						achievement_menu_3_page->items[89]->text = "--:--";
					}
				}
			}

			//int achievement_index = token.get<int>();
			//Game::Com_Printf(0, "^2%d index\n", achievement_index);
		});

		UIScript::Add("achievement_progressbar", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			Achievements::achievement_file_t file{};
			GetAchievementsData(&file);
			if (!Game::CL_IsCgameInitialized())
			{
				Game::menuDef_t* achievement_menu_1_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements").menu;
				Game::menuDef_t* achievement_menu_2_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_1").menu;
				Game::menuDef_t* achievement_menu_3_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_2").menu;

				int totalWidth = 493;
				int progressBarWidth = CalculateProgressBarWidth(totalWidth, Achievements::GetEarnedAchievementCount(&file), ACHIEVEMENT_TOTAL_COUNT);
				achievement_menu_1_page->items[170]->window.rectClient.w = progressBarWidth;
				achievement_menu_2_page->items[170]->window.rectClient.w = progressBarWidth;
				achievement_menu_3_page->items[100]->window.rectClient.w = progressBarWidth;

				achievement_menu_1_page->items[171]->text = VA_fake("%d/%d", Achievements::GetEarnedAchievementCount(&file), ACHIEVEMENT_TOTAL_COUNT);
				achievement_menu_2_page->items[171]->text = VA_fake("%d/%d", Achievements::GetEarnedAchievementCount(&file), ACHIEVEMENT_TOTAL_COUNT);
				achievement_menu_3_page->items[101]->text = VA_fake("%d/%d", Achievements::GetEarnedAchievementCount(&file), ACHIEVEMENT_TOTAL_COUNT);
			}
			else
			{
				Game::menuDef_t* achievement_menu_1_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_ingame").menu;
				Game::menuDef_t* achievement_menu_2_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_1_ingame").menu;
				Game::menuDef_t* achievement_menu_3_page = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "achievements_page_2_ingame").menu;

				int totalWidth = 493;
				int progressBarWidth = CalculateProgressBarWidth(totalWidth, Achievements::GetEarnedAchievementCount(&file), ACHIEVEMENT_TOTAL_COUNT);
				achievement_menu_1_page->items[164]->window.rectClient.w = progressBarWidth;
				achievement_menu_2_page->items[164]->window.rectClient.w = progressBarWidth;
				achievement_menu_3_page->items[94]->window.rectClient.w = progressBarWidth;

				achievement_menu_1_page->items[165]->text = VA_fake("%d/%d", Achievements::GetEarnedAchievementCount(&file), ACHIEVEMENT_TOTAL_COUNT);
				achievement_menu_2_page->items[165]->text = VA_fake("%d/%d", Achievements::GetEarnedAchievementCount(&file), ACHIEVEMENT_TOTAL_COUNT);
				achievement_menu_3_page->items[95]->text = VA_fake("%d/%d", Achievements::GetEarnedAchievementCount(&file), ACHIEVEMENT_TOTAL_COUNT);
			}
		});

		UIScript::Add("reset_achievements_progress", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			achievement_file_t file{};
			WriteAchievement(&file);
		});

		// Original function it's just null function on PC :>
		GSC::AddFunction("giveachievement", []
		{
			GiveAchievement(Game::Scr_GetString(0));
		}, false);
	}

	Achievements::~Achievements()
	{
	}
}