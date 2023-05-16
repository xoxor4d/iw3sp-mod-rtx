#include "STDInc.hpp"

// Achivements module by fedddddd adapted for IW3SP.
//https://github.com/fedddddd/h2-mod/blob/develop/src/client/component/achievements.cpp
namespace Components
{
	std::mutex file_mutex;

	std::array<Achievements::achievement_t, Achievements::ACHIEVEMENT_TOTAL_COUNT> Achievements::achievements =
	{
		achievement_t(ACHIEVEMENT_ALL_ACHIEVEMENTS, "ACHIEVEMENTS_COMPLETED"),
		achievement_t(ACHIEVEMENT_1, "EARN_A_WINGED_DAGGER"),
		achievement_t(ACHIEVEMENT_2, "MAKE_THE_JUMP"),
		achievement_t(ACHIEVEMENT_3, "COMPLETE_BLACKOUT"),
		achievement_t(ACHIEVEMENT_4, "SAVE_THE_BACON"),
		achievement_t(ACHIEVEMENT_5, "BRING_EM_HOME"),
		achievement_t(ACHIEVEMENT_6, "COMPLETE_VILLAGE_ASSAULT"),
		achievement_t(ACHIEVEMENT_7, "PIGGYBACK_RIDE"),
		achievement_t(ACHIEVEMENT_8, "DESPERATE_MEASURES"),
		achievement_t(ACHIEVEMENT_9, "WIN_THE_WAR"),
		achievement_t(ACHIEVEMENT_10, "LOOK_SHARP"),
		achievement_t(ACHIEVEMENT_11, "EARS_AND_EYES"),
		achievement_t(ACHIEVEMENT_12, "DOWN_BOY_DOWN"),
		achievement_t(ACHIEVEMENT_13, "NEW_SQUADRON_RECORD"),
		achievement_t(ACHIEVEMENT_14, "RESCUE_ROYCEWICZ"),
		achievement_t(ACHIEVEMENT_15, "YOUR_SHOW_SUCKS"),
		achievement_t(ACHIEVEMENT_16, "MAN_OF_THE_PEOPLE"),
		achievement_t(ACHIEVEMENT_17, "STRAIGHT_FLUSH"),
		achievement_t(ACHIEVEMENT_18, "GHILLIES_IN_THE_MIST"),
		achievement_t(ACHIEVEMENT_19, "MILE_HIGH_CLUB"),
		achievement_t(ACHIEVEMENT_20, "NO_REST_FOR_THE_WEARY"),
		achievement_t(ACHIEVEMENT_21, "WAR_HARDENED"),
		achievement_t(ACHIEVEMENT_22, "THE_PACKAGE"),
		achievement_t(ACHIEVEMENT_23, "THE_RESCUE"),
		achievement_t(ACHIEVEMENT_24, "THE_SEARCH"),
		achievement_t(ACHIEVEMENT_25, "THE_BOG"),
		achievement_t(ACHIEVEMENT_26, "THE_ESCAPE"),
		achievement_t(ACHIEVEMENT_27, "THE_FIRST_HORSEMAN"),
		achievement_t(ACHIEVEMENT_28, "THE_SECOND_HORSEMAN"),
		achievement_t(ACHIEVEMENT_29, "THE_SHOT"),
		achievement_t(ACHIEVEMENT_30, "THE_THIRD_HORSEMAN"),
		achievement_t(ACHIEVEMENT_31, "THE_ULTIMATUM"),
		achievement_t(ACHIEVEMENT_32, "THE_FOURTH_HORSEMAN"),
		achievement_t(ACHIEVEMENT_33, "DAREDEVIL"),
		achievement_t(ACHIEVEMENT_34, "ROADKILL"),
		achievement_t(ACHIEVEMENT_35, "BIRD_ON_THE_GROUND"),
		achievement_t(ACHIEVEMENT_36, "FOUR_OF_A_KIND"),
		achievement_t(ACHIEVEMENT_37, "THREE_OF_A_KIND"),
	};

	bool Achievements::HasAchievement(achievement_file_t* file, int id)
	{
		if (id >= ACHIEVEMENT_TOTAL_COUNT)
		{
			return false;
		}

		return file->achievements[id];
	}

	bool Achievements::HasAchievement(int id)
	{
		achievement_file_t file{};
		GetAchievementsData(&file);
		return HasAchievement(&file, id);
	}

	std::string Achievements::GetAchievementName(int id) { return Utils::String::VA("ACHIEVEMENT_NAME_%i", id); }
	std::string Achievements::GetAchievementDetails(int id) { return Utils::String::VA("ACHIEVEMENT_DETAIL_%i", id); }

	std::filesystem::path AchievementPath()
	{
		std::filesystem::path game_folder = Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string;

		auto fileBuffer = Utils::IO::ReadFile("players/profiles/active.txt");
		if (fileBuffer.empty())
			return "";

		return game_folder / Utils::String::VA("players\\profiles\\%s\\achievements.bin", fileBuffer.c_str());
	}

	std::optional<int> Achievements::GetAchievementID(const std::string& name)
	{
		for (auto i = 0; i < ACHIEVEMENT_TOTAL_COUNT; i++)
		{
			const auto achievement = &achievements[i];
			if (name == achievement->code)
			{
				return { achievement->id };
			}
		}

		return {};
	}

	void Achievements::WriteAchievement(achievement_file_t* data)
	{
		std::lock_guard _0(file_mutex);

		data->version = ACHIEVEMENT_FILE_VERSION;
		data->signature = ACHIEVEMENT_FILE_SIGNATURE;

		const auto path = AchievementPath();
		const auto str = std::string(reinterpret_cast<char*>(data), sizeof(achievement_file_t));
		Utils::IO::WriteFile(path.generic_string(), str, false);
	}

	bool Achievements::CheckThePlatinum(achievement_file_t* file)
	{
		if (HasAchievement(file, ACHIEVEMENT_ALL_ACHIEVEMENTS)) return false;

		//from 1 to 37
		for (auto i = static_cast<int>(ACHIEVEMENT_START); i < ACHIEVEMENT_TOTAL_COUNT; i++)
		{
			if (!file->achievements[i])
				return false;
		}

		return true;
	}

	void Achievements::GiveAchievementByID(achievement_file_t* file, int id)
	{
		GiveAchievementByID_Internal(file, id);
		if (CheckThePlatinum(file))
		{
			GiveAchievementByID_Internal(file, ACHIEVEMENT_ALL_ACHIEVEMENTS);
		}
	}

	void Achievements::GiveAchievement(const std::string& name)
	{
		Achievements::achievement_file_t file{};

		const auto id = GetAchievementID(name);
		if (!id.has_value())
		{
			return;
		}

		GiveAchievementByID(&file, id.value());
	}

	void Achievements::GetAchievementsData(achievement_file_t* file)
	{
		std::lock_guard _0(file_mutex);
		const auto path = AchievementPath().generic_string();

		if (!Utils::IO::FileExists(path)) return;

		const auto data = Utils::IO::ReadFile(path);
		if (data.size() < sizeof(achievement_file_t)) return;

		std::memcpy(file, data.data(), std::min(data.size(), sizeof(achievement_file_t)));
		if (file->signature != ACHIEVEMENT_FILE_SIGNATURE)
		{
			std::memset(file, 0, sizeof(achievement_file_t));
		}
	}

	void Achievements::GiveAchievementByID_Internal(achievement_file_t* file, int id)
	{
		GetAchievementsData(file);
		if (HasAchievement(file, id))
		{
			Game::Com_Printf(0, "^1Achievement Debug: you already unlock this achievement\n");
			return;
		}

		file->achievements[id] = true;
		file->timeStamp[id] = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		WriteAchievement(file);

		Game::Material* material = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MATERIAL, Utils::String::VA("trophy_%d", id)).material;
		Toast::Show(material, Game::UI_SafeTranslateString("ACHIEVEMENT_EARNED"), Game::UI_SafeTranslateString(GetAchievementName(id).c_str()), 4000, true);

		// Details may be too very long for the form.
		//Toast::Show(material, Game::UI_SafeTranslateString(get_name(id).c_str()), Game::UI_SafeTranslateString(get_details(id).c_str()), 4000, true);
	}

	Achievements::Achievements()
	{
		// Original function it's just null function on PC :>
		GSC::AddFunction("giveachievement", []
		{
			GiveAchievement(Game::Scr_GetString(0));
		}, false);
	}

	Achievements::~Achievements()
	{
	}
}