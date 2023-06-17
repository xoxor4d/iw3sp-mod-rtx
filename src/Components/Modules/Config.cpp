#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, Config::field_definition_t> Config::field_definitions =
	{
		{define_field("language", field_type::string, "english")},
		{define_field("language_first_setting", field_type::boolean, true)},
	};

	std::string Config::GetConfigFile()
	{
		std::string mod_config = "settings.json";
		std::string base_path = Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string + "\\iw3sp_data\\"s + mod_config.c_str();

		return base_path.c_str();
	}

	nlohmann::json Config::ValidateTheConfigField(const std::string& key, const nlohmann::json& value)
	{
		const auto iter = field_definitions.find(key);
		if (iter == field_definitions.end())
		{
			return value;
		}

		if (value.type() != iter->second.type)
		{
			return iter->second.default_value;
		}

		if (iter->second.validate_value.has_value())
		{
			const auto& validate_value = iter->second.validate_value.value();
			if (!validate_value(value))
			{
				iter->second.default_value;
			}
		}

		return value;
	}

	std::optional<nlohmann::json> Config::GetDefaultValue(const std::string& key)
	{
		const auto iter = field_definitions.find(key);
		if (iter == field_definitions.end())
		{
			return {};
		}

		return { iter->second.default_value };
	}

	nlohmann::json Config::GetRaw(const std::string& key)
	{
		const auto cfg = ReadConfig();
		if (!cfg.is_object() || !cfg.contains(key))
		{
			const auto default_value = GetDefaultValue(key);
			if (default_value.has_value())
			{
				return default_value.value();
			}

			return {};
		}

		return ValidateTheConfigField(key, cfg[key]);
	}

	void Config::WriteConfig(const nlohmann::json& json)
	{
		try
		{
			const auto path = GetConfigFile();
			const auto str = json.dump(4);
			Utils::IO::WriteFile(path, str, false);
		}
		catch (const std::exception& e)
		{
			Game::Com_Printf(0, "Failed to write config file: %s\n", e.what());
		}
	}

	nlohmann::json Config::ReadConfig()
	{
		const auto path = Config::GetConfigFile();
		if (!Utils::IO::FileExists(path))
		{
			return {};
		}

		try
		{
			const auto data = Utils::IO::ReadFile(path);
			return nlohmann::json::parse(data);
		}
		catch (const std::exception& e)
		{
			Game::Com_Printf(0, "Failed to parse config file: %s\n", e.what());
			Utils::IO::WriteFile(path, "{}", false);
		}

		return {};
	}

	void Config::CallExecFromCFG(bool useModConfig)
	{
		Command::Execute("disconnect");
		Utils::Hook::Call<void(std::uint32_t, int)>(0x589DB0)(0xFFFF, 0);
		const std::string& profileName = Utils::IO::ReadFile("players/profiles/active.txt");
		Dvars::Functions::Dvar_SetStringByName("com_playerProfile", profileName.c_str());

		char __str[64];
		if (!useModConfig)
		{
			Utils::Hook::Call<void(char*, int, char const*)>(0x532A40)(__str, 64, CLIENT_CONFIG);
		}
		else
		{
			Utils::Hook::Call<void(char*, int, char const*)>(0x532A40)(__str, 64, MOD_CONFIG);

			const std::string& moddir = Dvars::Functions::Dvar_FindVar("fs_game")->current.string;
			const std::string& profilePathStock = Utils::String::VA("players/profiles/%s/", profileName.c_str());

			// Reallocating settings from one .cfg to another
			if (!Utils::IO::FileExists(profilePathStock.c_str() + "DO_NOT_DELETE"s))
			{
				auto data = Utils::IO::ReadFile(profilePathStock.c_str() + "iw3sp_mod_config.cfg"s);
				Utils::IO::WriteFile(profilePathStock.c_str() + "mods_config.cfg"s, data);
				Utils::IO::WriteFile(profilePathStock.c_str() + "DO_NOT_DELETE"s, "// DO NOT DELETE THIS FILE IF YOU WANT KEEP THE SETTINGS IN 'mods_config.cfg'!", true);
			}
		}

		Game::Com_ExecStartupConfigs(Game::uiInfo->uiDC.localClientNum, __str);
	}

	void Config::ReplaceConfig(char* __str, signed __int32 __size, const char* __format, ...)
	{	
		const std::string& profileName = Utils::IO::ReadFile("players/profiles/active.txt");

		if (!Config::GameWithMod)
		{
			Utils::Hook::Call<void(char*, int, const char*)>(0x532A40)(__str, __size, CLIENT_CONFIG); //Com_BuildPlayerProfilePath

			const std::string& profilePath = Utils::String::VA("players/profiles/%s/", profileName.c_str());

			if (Utils::IO::FileExists(profilePath.c_str() + "config.cfg"s))
			{
				// Reallocating settings from one .cfg to another
				auto data = Utils::IO::ReadFile(profilePath.c_str() + "config.cfg"s);
				Utils::IO::WriteFile(profilePath.c_str() + "config_backup.cfg"s, data);
				Utils::IO::RemoveFile(profilePath.c_str() + "config.cfg"s);
				Utils::IO::RemoveFile(profilePath.c_str() + "iw3sp_mod_config.cfg"s);
				Utils::IO::WriteFile(profilePath.c_str() + "iw3sp_mod_config.cfg"s, data);
			}
		}
		else
		{
			Utils::Hook::Call<void(char*, int, const char*)>(0x532A40)(__str, __size, MOD_CONFIG); //Com_BuildPlayerProfilePath
		}
	}

	void __declspec(naked) Com_SetPlayerProfile_stub()
	{
		const static uint32_t retn_addr = 0x532788;
		__asm
		{
			call		Config::ReplaceConfig;
			jmp			retn_addr
		}
	}

	void __declspec(naked) Com_SetPlayerProfile_stub2()
	{
		const static uint32_t retn_addr = 0x53545A;
		__asm
		{
			call		Config::ReplaceConfig;
			jmp			retn_addr
		}
	}

	Config::Config()
	{
		if (!Config::GameWithMod)
		{
			Utils::Hook::Set<const char*>(0x5309CE, CLIENT_CONFIG);
			Utils::Hook::Set<const char*>(0x532778, CLIENT_CONFIG);
			Utils::Hook::Set<const char*>(0x53544A, CLIENT_CONFIG);
			Utils::Hook::Set<const char*>(0x57B2BD, CLIENT_CONFIG);
		}
		else
		{
			Utils::Hook::Set<const char*>(0x5309CE, MOD_CONFIG);
			Utils::Hook::Set<const char*>(0x532778, MOD_CONFIG);
			Utils::Hook::Set<const char*>(0x53544A, MOD_CONFIG);
			Utils::Hook::Set<const char*>(0x57B2BD, MOD_CONFIG);
		}

		Utils::Hook(0x532783, Com_SetPlayerProfile_stub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x535455, Com_SetPlayerProfile_stub2, HOOK_JUMP).install()->quick();

		// Check once time.
		Scheduler::Once([]
		{
			if(!Utils::IO::FileExists("iw3sp_data/settings.json"))
			{
				Config::Set("language", Config::GetRaw("language"));
				Config::Set("language_first_setting", Config::GetRaw("language_first_setting"));
			}
		}, Scheduler::Pipeline::MAIN);
	}

	Config::~Config()
	{
	}
}