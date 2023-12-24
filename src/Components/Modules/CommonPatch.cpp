#include "STDInc.hpp"

namespace Components
{
	void sv_cheats_hook()
	{
		Dvars::Override::DvarBoolOverride("sv_cheats", 
			Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled ? true : false, 
			Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled ? Game::none : Game::cheat_protected);
	}

	void DB_LoadCommonFastFiles()
	{
		int i = 0;
		Game::XZoneInfo XZoneInfoStack[6];

		XZoneInfoStack[i].name = "code_post_gfx";
		XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_POST_GFX;
		XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_POST_GFX_FREE;
		++i;

		const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);
		if (FastFiles::Exists(Utils::String::VA("%s_iw3sp_mod_patch", languageName)))
		{
			XZoneInfoStack[i].name = Utils::String::VA("%s_iw3sp_mod_patch", languageName);
			XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_MOD_FREE;
			++i;
			
			CommonPatch::iw3sp_mod_ff_exists = true;

			//override the exists the fonts from code_post_gfx
			if (Language::GetCurrentLanguage() == "russian")
			{
				Game::DB_LoadXAssets(XZoneInfoStack, i, false);
				Game::R_BeginRemoteScreenUpdate();
				WaitForSingleObject(Game::databaseCompletedEvent, 0xFFFFFFFF);
				Game::R_EndRemoteScreenUpdate();
				i = 0;

				Utils::Hook::Set<const char*>(0x42033E, "decode_characters_64px");
				Utils::Hook::Set<const char*>(0x420365, "decode_characters_64px_glow");
		}
		else
			{
				Utils::Hook::Set<const char*>(0x42033E, "decode_characters");
				Utils::Hook::Set<const char*>(0x420365, "decode_characters_glow");
			}
		}
		else
			CommonPatch::iw3sp_mod_ff_exists = false;

		if (FastFiles::Exists("iw3sp_mod"))
		{
			XZoneInfoStack[i].name = "iw3sp_mod";
			XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_MOD_FREE;
			++i;
			CommonPatch::iw3sp_mod_loc_ff_exists = true;
		}
		else
			CommonPatch::iw3sp_mod_loc_ff_exists = false;

		auto mod_exists = Utils::Hook::Call<char()>(0x45CC00)(); //DB_ModFileExists 0x45C920 from 1.0
		if (mod_exists)
		{
			// Unloading our mod except "loc_mod" file when mod.ff exists in mods/modName folder
			XZoneInfoStack[2].name = nullptr;
			XZoneInfoStack[2].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			XZoneInfoStack[2].freeFlags = Game::XZONE_FLAGS::XZONE_MOD_FREE;


			XZoneInfoStack[i].name = "mod";
			XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_MOD_FREE;
			++i;

			Game::DB_LoadXAssets(XZoneInfoStack, i, true);

			Game::R_BeginRemoteScreenUpdate();
			WaitForSingleObject(Game::databaseCompletedEvent, 0xFFFFFFFF);
			Game::R_EndRemoteScreenUpdate();
			i = 0;
		}

		XZoneInfoStack[i].name = "ui";
		XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_UI;
		XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_UI_FREE;
		++i;

		XZoneInfoStack[i].name = "common";
		XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_COMMON;
		XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_COMMON_FREE;
		++i;

		// < rtx begin

		if (FastFiles::Exists("xcommon_rtx"))
		{
			XZoneInfoStack[i].name = "xcommon_rtx";
			XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_COMMON;
			XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_ZERO;
			++i;
		}

		// rtx end >

		Game::DB_LoadXAssets(XZoneInfoStack, i, true);

		Game::R_BeginRemoteScreenUpdate();
		WaitForSingleObject(Game::databaseCompletedEvent, 0xFFFFFFFF);
		Game::R_EndRemoteScreenUpdate();

		Game::Com_Printf(0, "Game [^2Success^7]: main .ff files has been loaded.\n");

		Command::Add("test_cmd", [](Command::Params*)
		{
			Game::Com_Printf(0, "Test print message\n");
		});
		
		Command::Add("test_url_cmd", [](Command::Params*)
		{
			Utils::OpenUrl("https://youtu.be/dQw4w9WgXcQ?t=85");
		});

		Command::Add("unlockall", [](Command::Params*)
		{
			Command::Execute("seta mis_01 50");
			//Command::Execute("seta mis_difficulty 4444444444444444444440000000000000000000");
			//Command::Execute("seta cheat_points 30");
			//Command::Execute("seta cheat_items_set1 1073741823");
			Game::Com_Printf(0, "Game [^3INFO^7]: missions and cheats has been unlocked on your game profile.\n");
		});

		Command::Add("resetstats", [](Command::Params*)
		{
			Command::Execute("seta mis_01 0");
			Command::Execute("seta mis_difficulty 0000000000000000000000000000000000000000");
			Command::Execute("seta cheat_points 0");
			Command::Execute("seta cheat_items_set1 0");
			Game::Com_Printf(0, "Game [^3INFO^7]: missions and cheats has been resets on your game profile.\n");
		});

		Command::Add("applyCheat", [](Command::Params*)
		{
			if (Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled)
			{
				Command::Execute("seta thereisacow 1337", false);

				Utils::Hook::Set<BYTE>(0x587872, 0xEB); // Read only.
				Utils::Hook::Set<BYTE>(0x58788F, 0xEB); // Write protected.
				Utils::Hook::Set<BYTE>(0x5878AD, 0xEB); // Cheat protected.
			}
			else
			{
				Command::Execute("seta thereisacow 0", false);

				Utils::Hook::Set<BYTE>(0x587872, 0x74); // Read only.
				Utils::Hook::Set<BYTE>(0x58788F, 0x74); // Write protected.
				Utils::Hook::Set<BYTE>(0x5878AD, 0x75); // Cheat protected.
			}

			Dvars::Override::DvarBoolOverride("sv_cheats", 
				Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled ? true : false,
				Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled ? Game::none : Game::cheat_protected);
		});

		ServerCommand::Add("server_test_cmd", []()
		{
			Game::Com_Printf(0, "^8MY FIRST TEST SERVER COMMAND\n");
		});

		// Unlocked the developer_script flag from "write only" to "none"
		Dvars::Functions::Dvar_FindVar("developer_script")->flags = Game::none;
		Dvars::Functions::Dvar_FindVar("r_ignorehwgamma")->flags = Game::saved;

		const float newDefault_con_inputBoxColor[4] = { 0.15f, 0.15f, 0.15f, 1.0f };
		const float newDefault_con_inputHintBoxColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		const float newDefault_con_outputWindowColor[4] = { 0.1f, 0.1f, 0.1f, 0.75f };
		const float newDefault_con_outputBarColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		const float newDefault_con_outputSliderColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

		// I can't find more genius idea than this, but, anyway, this is works fine and correct for me!
		// For save values from user config.
		Dvars::Override::DvarVec4OverrideDefaultValue("con_inputBoxColor", newDefault_con_inputBoxColor);
		Dvars::Override::DvarVec4Override("con_inputBoxColor", Dvars::Functions::Dvar_FindVar("con_inputBoxColor")->current.vector);
		Dvars::Override::DvarVec4OverrideDefaultValue("con_inputHintBoxColor", newDefault_con_inputHintBoxColor);
		Dvars::Override::DvarVec4Override("con_inputHintBoxColor", Dvars::Functions::Dvar_FindVar("con_inputHintBoxColor")->current.vector);
		Dvars::Override::DvarVec4OverrideDefaultValue("con_outputWindowColor", newDefault_con_outputWindowColor);
		Dvars::Override::DvarVec4Override("con_outputWindowColor", Dvars::Functions::Dvar_FindVar("con_outputWindowColor")->current.vector);
		Dvars::Override::DvarVec4OverrideDefaultValue("con_outputBarColor", newDefault_con_outputBarColor);
		Dvars::Override::DvarVec4Override("con_outputBarColor", Dvars::Functions::Dvar_FindVar("con_outputBarColor")->current.vector);
		Dvars::Override::DvarVec4OverrideDefaultValue("con_outputSliderColor", newDefault_con_outputSliderColor);
		Dvars::Override::DvarVec4Override("con_outputSliderColor", Dvars::Functions::Dvar_FindVar("con_outputSliderColor")->current.vector);

		// < rtx begin
		Rtx::force_dvars_on_init();
		// rtx end >

		// Override existing localize entries.
		LocalizedStrings::OverrideLocalizeStrings();

		if (Flags::HasFlag("dump"))
		{
			Command::Add("dumpLocStrings", [](Command::Params* params)
			{
				if (params->size() < 2)
				{
					Game::Com_Printf(0, "dump_localize_strings <menu_name>\n");
					return;
				}

				std::vector<Game::LocalizeEntry*> localize;
				std::ofstream dumpFile;

				FastFiles::Enum_Assets(Game::ASSET_TYPE_LOCALIZE_ENTRY, [&localize](Game::XAssetHeader header)
				{
					localize.push_back(header.localize);
				}, false);

				// sort localize strings
				std::sort(localize.begin(), localize.end(), [](Game::LocalizeEntry* localize_start, Game::LocalizeEntry* localize_end)
				{
					return std::string_view(localize_start->name) < std::string_view(localize_end->name);
				});

				if (const auto& fs_basepath = Dvars::Functions::Dvar_FindVar("fs_basepath"); fs_basepath)
				{
					std::string base_path = fs_basepath->current.string;
					const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);
					base_path += Utils::String::VA("\\dump\\%s\\localizedstrings\\", languageName);

					if (std::filesystem::create_directories(base_path))
					{
						Game::Com_Printf(0, "Created directory \"dump/%s/localizedstrings\"\n", languageName);
					}

					const std::string file_name = base_path + Utils::String::VA("%s.str", params->get(1));
					dumpFile.open(file_name.c_str());
					if (dumpFile.is_open())
					{
						dumpFile << "// dumped by IW3SP_MOD " << __TIMESTAMP__ << "\n";
						dumpFile << "\n";

						dumpFile << "// Note to translators:\n";
						dumpFile << "// If a sentence is the same in your language then please change it to ""#same""\n";
						dumpFile << "//\n";
						dumpFile << "// eg:\n";
						dumpFile << "// LANG_ENGLISH  \"HALT\"\n";
						dumpFile << "// LANG_GERMAN  \"#same\"\n";
						dumpFile << "//\n";
						dumpFile << "// (This is so we can tell which strings have been signed-off as ok to be the same words for QA\n";
						dumpFile << "//  and because we do not store duplicate strings, which will then get exported again next time\n";
						dumpFile << "//  as being untranslated.)\n";
						dumpFile << "//\n";
						dumpFile << "VERSION             \"1\"\n";
						dumpFile << "CONFIG				 \"C:\\trees\\cod3\\cod3\\bin\\StringEd.cfg\"\n";
						dumpFile << "FILENOTES			 \"\"\n";
						dumpFile << "\n";

						for (std::size_t i = 0; i < localize.size(); i++)
						{
							if (Utils::String::StartsWith(localize[i]->name, Utils::String::ToUpper(params->get(1))))
							{
								//Utils::String::
								std::string localize_name = Utils::String::VA("%s", localize[i]->name);
								Utils::String::Replace(localize_name, Utils::String::ToUpper(params->get(1)) + "_", "");
								dumpFile << "REFERENCE           " << localize_name.c_str() << std::endl;
								dumpFile << "LANG_" << Utils::String::ToUpper(languageName) << "        " << Utils::String::VA("\"%s\"", localize[i]->value) << std::endl;
								dumpFile << "\n";
							}
						}

						dumpFile << "ENDMARKER\n";

						Game::Com_Printf(0, Utils::String::VA("Dumped localized strings to: %s", file_name.c_str()));
						dumpFile.close();
					}
				}
			});
		}

		UIScript::Add("testUIScript", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			Game::Com_Printf(0, "^2Game:: test uiscript\n");
		});

		UIScript::Add("VisitYT", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			Utils::OpenUrl("https://youtu.be/dQw4w9WgXcQ?t=85");
		});
	}

	void RunModStub()
	{
		char modName[260];

		if (Game::dword_129ADC4[2 * *Game::dword_129AFC8])
		{
			sprintf_s(modName, "%s/%s", "mods", (const char*)Game::dword_129ADC4[2 * *Game::dword_129AFC8]);
			Game::Com_Printf(0, "^2%s \n", modName);

			if (Dvars::Functions::Dvar_FindVar("useFastFile")->current.enabled)
			{
				Game::R_BeginRemoteScreenUpdate();
				WaitForSingleObject(Game::databaseCompletedEvent, 0xFFFFFFFF);
				Game::R_EndRemoteScreenUpdate();
			}

			Dvars::Functions::Dvar_SetStringByName("fs_game", modName);
			Language::VideoSubtitlesStub();
			Config::GameWithMod = true;
			Config::CallExecFromCFG(true);
		}
		}

	void ClearModStub()
	{
		Dvars::Functions::Dvar_Reset(0, Dvars::Functions::Dvar_FindVar("fs_game"));
		Language::VideoSubtitlesStub();
		Config::GameWithMod = false;
		Config::CallExecFromCFG(false);
	}

	void R_Cinematic_BinkOpen_stub01(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* fileName, const char* videoFormat)
	{
		const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);

		if (Utils::IO::FileExists(Utils::String::VA("%s\\main\\video\\%s.%s", directory, fileName, videoFormat)))
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\main\\video\\%s.%s", directory, fileName, videoFormat);
			return;
		}
		else if (Utils::IO::FileExists(Utils::String::VA("%s\\main\\%s_video\\%s.%s", directory, languageName, fileName, videoFormat)))
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\main\\%s_video\\%s.%s", directory, languageName, fileName, videoFormat);
			return;
		}
		else if (Utils::IO::FileExists(Utils::String::VA("%s\\%s\\video\\%s.%s", directory, Dvars::Functions::Dvar_FindVar("fs_game")->current.string, fileName, videoFormat)))
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\%s\\video\\%s.%s", directory, Dvars::Functions::Dvar_FindVar("fs_game")->current.string, fileName, videoFormat);
			return;
		}
		else
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\%s\\video\\%s\\%s.%s", directory, Dvars::Functions::Dvar_FindVar("fs_game")->current.string, languageName, fileName, videoFormat);
			return;
		}
	}

	void R_Cinematic_BinkOpen_stub02(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* fileName, const char* videoFormat)
	{
		const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);

		if (Utils::IO::FileExists(Utils::String::VA("%s\\raw\\video\\%s.%s", directory, fileName, videoFormat)))
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\raw\\video\\%s.%s", directory, fileName, videoFormat);
			return;
		}
		else
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\raw\\%s_video\\%s.%s", directory, languageName, fileName, videoFormat);
			return;
		}
	}

	void Com_Quit_f_stub()
	{
		Utils::Library::Terminate();
	}

	void LanguageSetValueConfig(int langIndex)
	{
		Config::Set<std::string>("language", Game::SEH_GetLanguageName(langIndex));
	}

	void __declspec(naked) updateLanguageStub()
	{
		const static uint32_t retn_addr = 0x5674FD;
		__asm
		{
			push	eax; // value
			call	Dvars::Functions::Dvar_SetIntByName;
			call	LanguageSetValueConfig;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	CommonPatch::CommonPatch()
	{
		Events::OnDvarInit([]
		{
			Dvars::external_console = Dvars::Register::Dvar_RegisterBool("external_console", "Turn on/off the external console", false, Game::saved);

			//TESTING DVARS
			// Bool dvar register test
			Dvars::test_dvar_bool = Dvars::Register::Dvar_RegisterBool("test_dvar_bool", "Test Bool Dvar", false, Game::none);
			// Int dvar register test
			Dvars::test_dvar_int = Dvars::Register::Dvar_RegisterInt("test_dvar_int", "Test Int Dvar", 0, 0, 3, Game::none);

			// Enum dvar register test
			static std::vector <const char*> r_enum_test_values = { "0", "1", "2", "3", };
			Dvars::test_dvar_enum = Dvars::Register::Dvar_RegisterEnum("test_dvar_enum", "Test Enum Dvar", 0, r_enum_test_values.size(), r_enum_test_values.data(), Game::none);

			// Float dvar register test
			Dvars::test_dvar_float = Dvars::Register::Dvar_RegisterFloat("test_dvar_float", "Test Float Dvar", 0.0f, 0.0f, 100.0f, Game::none);

			// Vectors dvars register test
			Dvars::test_dvar_vec2 = Dvars::Register::Dvar_RegisterVec2("test_dvar_vec2", "Test Vector2 Dvar", 0.0f, 0.0f, 0.0f, 25.0f, Game::none);
			Dvars::test_dvar_vec3 = Dvars::Register::Dvar_RegisterVec3("test_dvar_vec3", "Test Vector3 Dvar", 0.0f, 0.0f, 0.0f, 0.0f, 50.0f, Game::none);
			Dvars::test_dvar_vec4 = Dvars::Register::Dvar_RegisterVec4("test_dvar_vec4", "Test Vector4 Dvar", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100.0f, Game::none);

			// String DVAR Test
			Dvars::test_dvar_string = Dvars::Register::Dvar_RegisterString("test_dvar_string", "Test String Dvar.", "default", Game::saved);

			// Saved flag dvar register test (works on one map only)
			Dvars::test_dvar_bool_savedflag = Dvars::Register::Dvar_RegisterBool("test_dvar_savedbool", "Test Saved Bool Dvar", false, Game::saved_flag);

			// New fov control dvars
			Dvars::cg_fovScale = Dvars::Register::Dvar_RegisterFloat("cg_fovScale", "Scale applied to the field of view", 1.0f, 0.2f, 2.0f, Game::saved);
			Dvars::cg_fovMin = Dvars::Register::Dvar_RegisterFloat("cg_fovMin", "The minimum possible field of view", 10.0f, 1.0f, 160.0f, Game::saved);

			Dvars::ui_language = Dvars::Register::Dvar_RegisterInt("ui_language", "Game Language", 0, 0, 14, Game::none);
			Dvars::ui_debugMode = Dvars::Register::Dvar_RegisterBool("ui_debugMode", "Shows ui debug information on screen.", 0, 4u);

			// Now "cg_fov" dvar can saved without resets, but in jeepride game anyways will be resets to 65 by script.
			//Dvars::cg_fov = Dvars::Register::Dvar_RegisterFloat("cg_fov", "The field of view angle in degrees", 65.0f, 1.0f, 160.0f, Game::saved);

			//for 'main_lockout' menu
			Game::dvar_s* ui_skipMainLockout = Dvars::Register::Dvar_RegisterBool("ui_skipMainLockout", "", false, Game::none);
			Game::dvar_s* profile_unlock_all = Dvars::Register::Dvar_RegisterBool("profile_unlock_all", "", false, Game::saved);
			Game::dvar_s* cg_drawBrandingInfo = Dvars::Register::Dvar_RegisterBool("cg_drawBrandingInfo", "Show/Hide the branding text on the top-left screen", true, Game::saved);
			Game::dvar_s* sv_allowCheats = Dvars::Register::Dvar_RegisterBool("sv_allowCheats", "Enable/Disable the game cheats", false, Game::saved);
			//Game::dvar_s* ui_console_menu_style = Dvars::Register::Dvar_RegisterBool("ui_console_menu_style", "Enable/Disable menu style from console version", false, Game::none);
		});

		Utils::Hook(0x4B83CF, sv_cheats_hook, HOOK_CALL).install()->quick();
		Utils::Hook::Nop(0x5899E8, 5);

		Utils::Hook::Set<const char*>(0x445667, "iw3sp_mod.exe"); //-startSingleplayer
		//Utils::Hook::Set<BYTE>(0x41D652, Game::saved);
		//Utils::Hook::Set<BYTE>(0x41D653, 0x0);

		// g_mem limit expansion. Thanks to Nikolai for offsets.
		Utils::Hook::Set<DWORD>(0x534B3F, 0x20000000); //528mb
		Utils::Hook::Set<DWORD>(0x534B70, 0x20000000); //528mb

		// Increase hunkTotal from 10mb to 15mb (xoxor4d)
		Utils::Hook::Set<BYTE>(0x5800D9, 0xF0);

		// Increase g_copyInfo limit.
		Utils::Hook::Set<BYTE>(0x45B5C4, 0x16);

		// Fastfiles Hooking The Function
		Utils::Hook(0x5D94D0, DB_LoadCommonFastFiles, HOOK_CALL).install()->quick();

		// RunMod fix
		Utils::Hook::Nop(0x567238, 5);
		Utils::Hook::Nop(0x566E87, 5);
		Utils::Hook(0x567238, RunModStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x56727E, ClearModStub, HOOK_CALL).install()->quick();

		Utils::Hook::Set<BYTE>(0x57AB0C, Game::none);
		Utils::Hook::Set<const char*>(0x567241, "vid_restart\n");
		Utils::Hook::Set<const char*>(0x567284, "vid_restart\n"); //for ClearMods

		// Making the separate video folder
		Utils::Hook(0x5D70DA, R_Cinematic_BinkOpen_stub01, HOOK_CALL).install()->quick();
		Utils::Hook(0x5D70FC, R_Cinematic_BinkOpen_stub02, HOOK_CALL).install()->quick();

		Utils::Hook(0x5674F8, updateLanguageStub, HOOK_JUMP).install()->quick();

		// Com_Quit_f_stub for auto-update.
		Utils::Hook(0x595324, Com_Quit_f_stub, HOOK_CALL).install()->quick();

		// Increase fps cap to 250 for menus.
		Utils::Hook::Set<BYTE>(0x535881, 0xEB);
		Utils::Hook::Set<BYTE>(0x535884, 0xEB);

		// Fix fps on windows 10 (stuck at 500) :: sleep(1) to sleep(0) (xoxor4d)
		Utils::Hook::Set<BYTE>(0x53571F, 0x0);

		// Alt + Enter for switching from fullscreen2window mode or window2fullscreen without developer mode.
		Utils::Hook::Nop(0x596BBE, 2);

		// Changed function for notice that player activate godmode/notarget and etc.
		Utils::Hook::Set<const char*>(0x4AD009, "gm \"%s\""); //godmode ON/OFF
		Utils::Hook::Set<const char*>(0x4AD099, "gm \"%s\""); //demigod ON/OFF
		Utils::Hook::Set<const char*>(0x4AD129, "gm \"%s\""); //notarget ON/OFF
		Utils::Hook::Set<const char*>(0x4AD1C4, "gm \"%s\""); //noclip ON/OFF
		Utils::Hook::Set<const char*>(0x4AD264, "gm \"%s\""); //ufo ON/OFF

		// Replaced the original .cfg name file.
		Utils::Hook::Set<const char*>(0x5309CE, CLIENT_CONFIG);
		Utils::Hook::Set<const char*>(0x532778, CLIENT_CONFIG);
		Utils::Hook::Set<const char*>(0x53544A, CLIENT_CONFIG);
		Utils::Hook::Set<const char*>(0x57B2BD, CLIENT_CONFIG);

		static float BuildVersionColor[] = { 0.00f, 0.80f, 0.00f, 1.00f };
		Utils::Hook::Set<float*>(0x447661, BuildVersionColor);
		Utils::Hook::Set<float*>(0x43A8D2, BuildVersionColor);

		// Changing the font in video cinematic from 'normalFont' to 'extraBigFont'.
		Utils::Hook::Set<DWORD>(0x56502B, 0x6ABEC4);
		Utils::Hook::Nop(0x4477A1, 4);

		Utils::Hook::Nop(0x44045E, 5); // MAX_PACKET_USERCMDS\n
		Utils::Hook::Nop(0x53A334, 5); // Ragdoll allocation failed, out of ragdoll bodies (obj %d)

		// ui_showList: changing the font.
		Utils::Hook::Set<DWORD>(0x57164F, 0x129ADB8);

		// Mouse fix
		Utils::Hook::Nop(0x59681C, 8);
		Scheduler::Loop([]
		{
			SetThreadExecutionState(ES_DISPLAY_REQUIRED);
		}, Scheduler::Pipeline::MAIN);

		Scheduler::Loop([]
		{
			Utils::Hook::Set<bool>(0x1E209D4, (Dvars::ui_debugMode->current.enabled) ? 1 : 0);
		}, Scheduler::Pipeline::MAIN, 50ms);
	}
}