#include "STDInc.hpp"
#include <discord_rpc.h>

namespace Components
{
	DiscordRichPresence discord_presence;
	bool Discord_Presence_Initialized = false;

	static const char* stock_sp_map_names[] =
	{
		"killhouse",
		"cargoship",
		"coup",
		"blackout",
		"armada",
		"bog_a",
		"hunted",
		"ac130",
		"bog_b",
		"airlift",
		"aftermath",
		"village_assault",
		"scoutsniper",
		"sniperescape",
		"village_defend",
		"ambush",
		"icbm",
		"launchfacility_a",
		"launchfacility_b",
		"jeepride",
		"simplecredits",
		"airplane",
	};

	//	Check via offset const char is more genius, but this works fine than with CL_IsCgameInitialized
	bool InMainMenu(std::string map_name_info)
	{
		if (map_name_info == "") return true;
		else return false;
	}

	bool VanillaMap(std::string map_name_info)
	{
		for (const auto& map_name : stock_sp_map_names)
		{
			if (map_name == map_name_info)
				return true;
		}
		return false;
	}

	std::string GetDifficultyForState(std::string map_name_info)
	{
		bool user_in_main_menu = InMainMenu(map_name_info);

		if (!user_in_main_menu)
		{
			const auto difficulty = Dvars::Functions::Dvar_FindVar("g_gameskill")->current.integer;
			if (difficulty == 0)
				return "IW3SP_MOD_LOC_DISCORD_DIFFICULTY_EASY";
			else if (difficulty == 1)
				return "IW3SP_MOD_LOC_DISCORD_DIFFICULTY_MEDIUM";
			else if (difficulty == 2)
				return "IW3SP_MOD_LOC_DISCORD_DIFFICULTY_HARD";
			else if (difficulty == 3)
				return "IW3SP_MOD_LOC_DISCORD_DIFFICULTY_FU";
			else
				return "IW3SP_MOD_LOC_DISCORD_DIFFICULTY_UNKNOWN";
		}
		else
			return "";
	}

	std::string GetMapName(std::string map_name_info)
	{
		bool vanilla_map = VanillaMap(map_name_info);
		if (vanilla_map)
		{
			if (!strcmp(map_name_info.data(), "ac130") && Dvars::Functions::Dvar_FindVar("credits_active")->current.integer == 1)
			{
				return "IW3SP_MOD_LOC_DISCORD_SIMPLECREDITS";
			}

			return Utils::String::VA("IW3SP_MOD_LOC_DISCORD_%s", map_name_info.data());
		}
		else
		{
			bool user_in_main_menu = InMainMenu(map_name_info);
			if (user_in_main_menu) return "IW3SP_MOD_LOC_DISCORD_MAINMENU";
			else return "custom_map";
		}
	}

	void Discord::DiscordUpdate()
	{
		Discord_RunCallbacks();

		std::string map_name_offset = reinterpret_cast<const char*>(0x1290DD8);
		std::string mapname = GetMapName(map_name_offset.data());
		std::string difficulty = GetDifficultyForState(map_name_offset.data());

		if (strcmp(mapname.c_str(), "custom_map") == 0)
			discord_presence.details = Game::UI_ReplaceConversionString(Game::UI_SafeTranslateString("IW3SP_MOD_LOC_DISCORD_MAPUNKNOWN"), map_name_offset.c_str());
		else
			discord_presence.details = Game::UI_SafeTranslateString(mapname.data());

		if(difficulty.data()) discord_presence.state = Game::UI_SafeTranslateString(difficulty.data());
		else discord_presence.state = nullptr;

		bool user_in_main_menu = InMainMenu(map_name_offset.data());
		if(user_in_main_menu) 
			discord_presence.largeImageKey = "preview_mainmenu";
		else
		{
			bool vanilla_map = VanillaMap(map_name_offset.data());
			if (vanilla_map) discord_presence.largeImageKey = Utils::String::VA("preview_%s", map_name_offset.data());
			else discord_presence.largeImageKey = nullptr;
		}

		if (!discord_presence.startTimestamp)
		{
			discord_presence.startTimestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}

		Discord_UpdatePresence(&discord_presence);
	}

	static void ready([[maybe_unused]] const DiscordUser* request)
	{
		ZeroMemory(&discord_presence, sizeof(discord_presence));

		discord_presence.instance = 1;
		discord_presence.state = "";

		Discord_UpdatePresence(&discord_presence);
	}

	static void Errored(const int error_code, const char* message)
	{
		Game::Com_Printf(0, "Discord: (%i) %s", error_code, message);
	}

	void Discord::DiscordInit()
	{
		if (EnableDiscord_RPC && !Discord_Presence_Initialized)
		{
			DiscordEventHandlers handlers;
			ZeroMemory(&handlers, sizeof(handlers));
			handlers.ready = ready;
			handlers.errored = Errored;
			handlers.disconnected = Errored;
			handlers.joinGame = nullptr;
			handlers.spectateGame = nullptr;
			handlers.joinRequest = nullptr;

			Discord_Initialize("1088866597940248626", &handlers, 1, nullptr);

			Discord_Presence_Initialized = true;

			Scheduler::Loop(Discord::DiscordUpdate, Scheduler::Pipeline::MAIN, 5s);
		}
	}

	void Discord::DiscordShutdown()
	{
		if (Discord_Presence_Initialized)
		{
			Discord_Shutdown();
			Discord_Presence_Initialized = false;
		}
	}

	Discord::Discord()
	{
		Discord::DiscordInit();
	}

	Discord::~Discord()
	{
		Discord::DiscordShutdown();
	}
}