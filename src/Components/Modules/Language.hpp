#pragma once

namespace Components
{
	class Language : public Component
	{
	public:
		Language();
		~Language();
		static std::string GetCurrentLanguage();
	private:
		static void LanguageInstallStub();
		static void LanguageSetupInit();
		static void IntroSubtitlesStub();
	};
}