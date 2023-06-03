#pragma once

#define ACHIEVEMENT_FILE_VERSION 1
#define ACHIEVEMENT_FILE_SIGNATURE 'M3WI'

namespace Components
{
	class Achievements : public Component
	{
	public:
		Achievements();
		~Achievements();
	private:
		//Original IW3 achievements from Xbox 360.
		//https://callofduty.fandom.com/wiki/Achievements/Call_of_Duty_4:_Modern_Warfare
		enum achievement_id
		{
			ACHIEVEMENT_ALL_ACHIEVEMENTS = 0,
			ACHIEVEMENT_START = 1,
			ACHIEVEMENT_1 = 1,
			ACHIEVEMENT_2,
			ACHIEVEMENT_3,
			ACHIEVEMENT_4,
			ACHIEVEMENT_5,
			ACHIEVEMENT_6,
			ACHIEVEMENT_7,
			ACHIEVEMENT_8,
			ACHIEVEMENT_9,
			ACHIEVEMENT_10,
			ACHIEVEMENT_11,
			ACHIEVEMENT_12,
			ACHIEVEMENT_13,
			ACHIEVEMENT_14,
			ACHIEVEMENT_15,
			ACHIEVEMENT_16,
			ACHIEVEMENT_17,
			ACHIEVEMENT_18,
			ACHIEVEMENT_19,
			ACHIEVEMENT_20,
			ACHIEVEMENT_21,
			ACHIEVEMENT_22,
			ACHIEVEMENT_23,
			ACHIEVEMENT_24,
			ACHIEVEMENT_25,
			ACHIEVEMENT_26,
			ACHIEVEMENT_27,
			ACHIEVEMENT_28,
			ACHIEVEMENT_29,
			ACHIEVEMENT_30,
			ACHIEVEMENT_31,
			ACHIEVEMENT_32,
			ACHIEVEMENT_33,
			ACHIEVEMENT_34,
			ACHIEVEMENT_35,
			ACHIEVEMENT_36,
			ACHIEVEMENT_37,
			ACHIEVEMENT_TOTAL_COUNT,
		};

		struct achievement_t
		{
			achievement_id id;
			std::string_view code;
		};

		struct achievement_file_t
		{
			std::uint32_t signature;
			std::uint8_t version;
			bool achievements[ACHIEVEMENT_TOTAL_COUNT];
			std::time_t timeStamp[ACHIEVEMENT_TOTAL_COUNT];
		};

		static bool HasAchievement(int id);
		static bool HasAchievement(achievement_file_t* file, int id);
		static std::string GetAchievementName(int id);
		static std::string GetAchievementDetails(int id);
		static bool CheckThePlatinum(achievement_file_t* file);
		static std::optional<int> GetAchievementID(const std::string& name);
		static void GiveAchievementByID_Internal(achievement_file_t* file, int id);
		static void GiveAchievementByID(achievement_file_t* file, int id);
		static void GetAchievementsData(achievement_file_t* file);
		static void WriteAchievement(achievement_file_t* data);
		static void GiveAchievement(const std::string& name);
		static int GetEarnedAchievementCount(achievement_file_t* file);

		static std::array<Achievements::achievement_t, Achievements::ACHIEVEMENT_TOTAL_COUNT> achievements;
	};
}