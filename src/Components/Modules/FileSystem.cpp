#include "STDInc.hpp"

namespace Components
{
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

	FileSystem::FileSystem()
	{
	}

	FileSystem::~FileSystem()
	{
	}
}