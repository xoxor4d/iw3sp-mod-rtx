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

	void Config::ReplaceConfig(char* __str, signed __int32 __size, const char* __format, ...)
	{
		//original function
		Utils::Hook::Call<int(char*, int, const char*)>(0x532A40)(__str, __size, __format); //Com_SetPlayerProfile

		const std::string& profileName = Utils::IO::ReadFile("players/profiles/active.txt");
		const std::string& profilePath = Utils::String::VA("players/profiles/%s/", profileName.c_str());

		if (Utils::IO::FileExists(profilePath.c_str() + "config.cfg"s))
		{
			auto data = Utils::IO::ReadFile(profilePath.c_str() + "config.cfg"s);
			Utils::IO::WriteFile(profilePath.c_str() + "config_backup.cfg"s, data);
			Utils::IO::RemoveFile(profilePath.c_str() + "config.cfg"s);
			Utils::IO::RemoveFile(profilePath.c_str() + "iw3sp_mod_config.cfg"s);
			Utils::IO::WriteFile(profilePath.c_str() + "iw3sp_mod_config.cfg"s, data);
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
		//Com_SetPlayerProfile
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