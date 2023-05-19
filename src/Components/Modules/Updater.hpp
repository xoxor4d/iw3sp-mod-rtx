#pragma once

#define MASTER "https://master.iw3spmod.site/"
#define FILES_PATH "files.json"

namespace Components
{
	class Updater : public Component
	{
	public:
		static bool UpdateRestart;

		struct file_data
		{
			std::string name;
			std::string data;
		};

		struct file_info
		{
			std::string name;
			std::string hash;
		};

		struct status
		{
			bool done;
			bool success;
		};

		struct update_data_t
		{
			bool restart_required{};
			bool cancelled{};
			status check{};
			status download{};
			std::string error{};
			std::string current_file{};
			std::vector<file_info> required_files{};
			std::vector<std::string> garbage_files{};
		};

		Updater();
		~Updater();
	private:

		static void SetUpdateDownloadStatus(bool done, bool success);
		static bool CheckFile(const std::string& name, const std::string& sha);
		static bool WriteFile(const std::string& name, const std::string& data);
		static std::string get_time_str();
		static std::optional<std::string> DownloadFile(const std::string& name);
		static std::vector<std::string> FindGarbageFiles(const std::vector<std::string>& update_files);
		static void ResetData();
		static void CancelUpdate();
		static bool UpdateAvailable();
		static bool UpdateCancelled();

		static void CL_StartUpdate();
		static void CL_GetAutoUpdate();

		static Utils::ConcurrentList::Container<update_data_t> update_data;
	};
}