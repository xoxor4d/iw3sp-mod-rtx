#include "STDInc.hpp"

namespace Components
{
	std::string Language::GetCurrentLanguage()
	{
		return Config::Get<std::string>("language").value();
	}

	void Language::LanguageSetupInit()
	{
		if (Config::Get<bool>("language_first_setting").value() == false)
		{
			int langIndexDef = 0;
			const char* language = Game::SEH_GetLanguageName(Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef) );
			Game::localization->language = language;

			Dvars::Functions::Dvar_FindVar("loc_language")->current.integer = Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);
			Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer = Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);
			Dvars::Functions::Dvar_FindVar("ui_language")->current.integer = Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);
			Dvars::Functions::Dvar_FindVar("ui_language")->latched.integer = Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);
		}
		else if (Config::Get<bool>("language_first_setting").value() == true)
		{
			const char* language_buffer = reinterpret_cast<const char*>(0x13E0708);
			int langIndexDef = 0;

			Config::Set<std::string>("language", language_buffer);

			Dvars::Functions::Dvar_FindVar("loc_language")->current.integer = Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);
			Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer = Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);
			Dvars::Functions::Dvar_FindVar("ui_language")->current.integer = Dvars::Functions::Dvar_FindVar("loc_language")->current.integer;
			Dvars::Functions::Dvar_FindVar("ui_language")->latched.integer = Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer;

			Config::Set<bool>("language_first_setting", false);
		}
		else
		{
			Config::Set<bool>("language_first_setting", true);

			const char* language_buffer = reinterpret_cast<const char*>(0x13E0708);
			int langIndexDef = 0;

			Config::Set<std::string>("language", language_buffer);

			Dvars::Functions::Dvar_FindVar("loc_language")->current.integer = Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);
			Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer = Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);
			Dvars::Functions::Dvar_FindVar("ui_language")->current.integer = Dvars::Functions::Dvar_FindVar("loc_language")->current.integer;
			Dvars::Functions::Dvar_FindVar("ui_language")->latched.integer = Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer;

			Config::Set<bool>("language_first_setting", false);
		}
	}
	
	void Language::IntroSubtitlesStub()
	{
		if (Language::GetCurrentLanguage() == "english")
		{
			Utils::Hook::Set<const char*>(0x420F45, "video/vidsubtitles.csv");
			Utils::Hook::Set<const char*>(0x567AA4, "video/vidsubtitles.csv");
		}
		else
		{
			Utils::Hook::Set<const char*>(0x420F45, "video/subtitles.csv");
			Utils::Hook::Set<const char*>(0x567AA4, "video/subtitles.csv");
		}
	}

	void Language::LanguageInstallStub()
	{
		Language::LanguageSetupInit();
		Language::IntroSubtitlesStub();
		Utils::Hook::Call<std::uint8_t()>(0x614640)();
	}

	Language::Language()
	{
		Utils::Hook(0x5D9BC4, Language::LanguageInstallStub, HOOK_CALL).install()->quick();
	}

	Language::~Language()
	{
	}
}