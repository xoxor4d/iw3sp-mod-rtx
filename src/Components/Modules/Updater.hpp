#pragma once

#define MASTER "https://master.iw3spmod.site/"
#define FILES_PATH "files.json"

namespace Components
{
	class Updater : public Component
	{
	public:
		static bool UpdateRestart;
		static bool AutomaticUpdate;

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

		static bool UpdateCancelled();

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

		static void CL_StartUpdate();
		static void CL_GetAutoUpdate(bool needCheck);
		static void DeleteOldFiles();

		static Utils::ConcurrentList::Container<update_data_t> update_data;
	};
}