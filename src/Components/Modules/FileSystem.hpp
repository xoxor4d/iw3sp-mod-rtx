#pragma once

namespace Components
{
	class FileSystem : public Component
	{
	public:
		static void FS_AddFolder(const std::string& path);
		FileSystem();
		~FileSystem();
		static std::filesystem::path GetAppdataPath();
	private:
		static std::vector<std::string> FS_Folders;
		static void FS_DisplayPath_Stub();
	};
}