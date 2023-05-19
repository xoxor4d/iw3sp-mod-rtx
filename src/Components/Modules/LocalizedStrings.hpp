#pragma once

namespace Components
{
	class LocalizedStrings : public Component
	{
	public:
		static void OverrideLocalizeStrings();
		static std::optional<std::string> PrefixOverride;

		LocalizedStrings();
		~LocalizedStrings();

		static void Set(const std::string& psLocalReference, const std::string& psNewString);
		static const char* Get(const char* key);
	private:
		static std::recursive_mutex LocalizeMutex;
		static std::unordered_map<std::string, Game::LocalizeEntry*> LocalizeMap;
		static std::function<void(Game::LocalizeEntry*)> ParseCallback;

		static void __stdcall SetStringStub(const char* psLocalReference, const char* psNewString, int bSentenceIsEnglish);
		static void SaveParseOutput(Game::LocalizeEntry* asset);
	};
}