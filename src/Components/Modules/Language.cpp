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

	const char* Language::GetLanguageForVidSubtitles()
	{
		int language = Dvars::Functions::Dvar_FindVar("loc_language")->current.integer;

		switch (language)
		{
			case 0: return "video/en/vidsubtitles.csv"; break;
			case 1: return "video/fr/vidsubtitles.csv"; break;
			case 2: return "video/ge/vidsubtitles.csv"; break;
			case 3: return "video/it/vidsubtitles.csv"; break;
			case 4: return "video/sp/vidsubtitles.csv"; break;
			case 5: return "video/br/vidsubtitles.csv"; break;
			case 6: return "video/ru/vidsubtitles.csv"; break;
			case 7: return "video/po/vidsubtitles.csv"; break;
			case 8: return "video/ko/vidsubtitles.csv"; break;
			case 9: return "video/tw/vidsubtitles.csv"; break;
			case 10: return "video/jp/vidsubtitles.csv"; break;
			case 11: return "video/cn/vidsubtitles.csv"; break;
			case 12: return "video/th/vidsubtitles.csv"; break;
			case 13: return "video/1337/vidsubtitles.csv"; break;
			case 14: return "video/cz/vidsubtitles.csv"; break;
			default: return "video/en/vidsubtitles.csv"; break;
		}
	}
	
	void Language::VideoSubtitlesStub()
	{
		if (Language::GetCurrentLanguage() == "english" ||
			Language::GetCurrentLanguage() == "french"	||
			Language::GetCurrentLanguage() == "german"	||
			Language::GetCurrentLanguage() == "italian" ||
			Language::GetCurrentLanguage() == "russian" ||
			Language::GetCurrentLanguage() == "spanish")
		{
			Utils::Hook::Set<const char*>(0x420F45, GetLanguageForVidSubtitles());
			Utils::Hook::Set<const char*>(0x567AA4, GetLanguageForVidSubtitles());
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
		Language::VideoSubtitlesStub();
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