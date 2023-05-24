#include "STDInc.hpp"

namespace Components
{
	std::vector<std::string> FileSystem::FS_Folders;

	std::filesystem::path FileSystem::GetAppdataPath()
	{
		PWSTR path;
		if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path)))
		{
			throw std::runtime_error("Failed to read APPDATA path!");
		}

		auto _0 = gsl::finally([&path]
		{
			CoTaskMemFree(path);
		});

		return std::filesystem::path(path) / "iw3sp_mod";
	}

	void FileSystem::FS_AddFolder(const std::string& path)
	{
		FileSystem::FS_Folders.push_back(path);
	}

	void FileSystem::FS_DisplayPath_Stub()
	{
		std::string fs_basepath = Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string;

		std::vector<std::string> FileSystemFolders;
		Utils::Merge(&FileSystemFolders, FileSystem::FS_Folders);

		for (auto& FileSystemFolder : FileSystemFolders)
		{
			Game::FS_AddLocalizedGameDirectory(FileSystemFolder.data(), fs_basepath.data());
		}

		Game::FS_DisplayPath(1);
	}

	FileSystem::FileSystem()
	{
		FS_AddFolder("iw3sp_data");
		Utils::Hook(0x57B008, FileSystem::FS_DisplayPath_Stub, HOOK_CALL).install()->quick();
	}

	FileSystem::~FileSystem()
	{
	}
}