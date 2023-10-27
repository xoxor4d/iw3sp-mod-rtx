#pragma once

namespace Components
{
	class Changelog : public Component
	{
	public:
		enum ChangelogStatus
		{
			CHANGELOG_ERROR = 0,
			CHANGELOG_SUCCESS,
		};

		struct Changelog_t
		{
			ChangelogStatus status;
			std::string changeLogText;
		};

		static void CreateChangeLog(Game::gui_menus_t& menu);
		Changelog();
		~Changelog();
	private:
		static Changelog_t changeLogData;
	};
}