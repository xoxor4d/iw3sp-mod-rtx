#pragma once

namespace Components
{
	class Language : public Component
	{
	public:
		Language();
		~Language();
		static std::string GetCurrentLanguage();
		static void VideoSubtitlesStub();
	private:
		static const char* GetLanguageForVidSubtitles();
		static void LanguageInstallStub();
		static void LanguageSetupInit();
	};
}