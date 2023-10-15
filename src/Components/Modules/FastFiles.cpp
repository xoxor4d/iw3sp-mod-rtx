#include "STDInc.hpp"

namespace Components
{
	std::vector<std::string> FastFiles::ZonePaths;

	void FastFiles::Enum_Assets(const Game::XAssetType type, const std::function<void(Game::XAssetHeader)>& callback, const bool includeOverride)
	{
		Game::DB_EnumXAssets(type, static_cast<void(*)(Game::XAssetHeader, void*)>([](Game::XAssetHeader header, void* data)
			{
				const auto& cb = *static_cast<const std::function<void(Game::XAssetHeader)>*>(data);
		cb(header);
			}), &callback, includeOverride);
	}

	const char* FastFiles::GetZoneLocation(const char* file)
	{
		const auto& dir = Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string;
		std::vector<std::string> paths;
		std::string modDir = Dvars::Functions::Dvar_FindVar("fs_game")->current.string;
		if ((file == "mod"s || file == "mod.ff"s) && !modDir.empty())
		{
			paths.push_back(Utils::String::VA("%s\\", modDir.data()));
		}


		std::string zone = file;
		if (Utils::String::EndsWith(zone, ".ff"))
		{
			Utils::String::Replace(zone, ".ff", "");
		}

		std::string filename = zone;

		Utils::Merge(&paths, FastFiles::ZonePaths);

		for (auto& path : paths)
		{
			std::string absoluteFile = Utils::String::VA("%s\\%s%s", dir, path.data(), file);

			// No ".ff" appended, append it manually
			if (!Utils::String::EndsWith(absoluteFile, ".ff"))
			{
				absoluteFile.append(".ff");
			}

			// Check if FastFile exists
			if (Utils::IO::FileExists(absoluteFile))
			{
				return Utils::String::VA("%s", path.data());
			}
		}

		//english by default;
		const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);
		return Utils::String::VA("zone\\%s\\", languageName);
	}

	bool FastFiles::Exists(const std::string& file)
	{
		std::string path = FastFiles::GetZoneLocation(file.data());
		path.append(file);

		if (!Utils::String::EndsWith(path, ".ff"))
		{
			path.append(".ff");
		}

		return Utils::IO::FileExists(path);
	}

	bool FastFiles::DB_IsZoneLoaded(const char* zone)
	{
		for (int i = 1; i < 33; ++i)
		{
			if (Game::g_zoneNames[i].name[0] && !_stricmp(Game::g_zoneNames[i].name, zone))
				return Game::g_zoneNames[i].loaded;
		}

		return false;
	}

	void FastFiles::AddZonePath(const std::string& path)
	{
		FastFiles::ZonePaths.push_back(path);
	}

	void FastFiles::SetupZoneFolder(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* language, const char* zoneName)
	{
		// Looks similar with FastFiles::GetZoneLocation method, but need sprintf_s return
		for (auto& path : FastFiles::ZonePaths)
		{
			if (Utils::IO::FileExists(Utils::String::VA("%s\\%s\\%s.ff", directory, path.c_str(), zoneName)))
			{
				sprintf_s(buffer, size, "%s\\%s\\%s.ff", directory, path.data(), zoneName);
				return;
			}
		}

		sprintf_s(buffer, size, "%s\\zone\\%s\\%s.ff", directory, language, zoneName);
	}

	void __declspec(naked) DB_TryLoadXFileInternal_stub()
	{
		const static uint32_t retn_addr = 0x45BFEC;
		__asm
		{
			push	esi; // size
			push	edi; // fileName
			call	FastFiles::SetupZoneFolder;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	FastFiles::FastFiles()
	{
		// Custom zone folder
		Utils::Hook(0x45BFE7, DB_TryLoadXFileInternal_stub, HOOK_JUMP).install()->quick();

		FastFiles::AddZonePath("iw3sp_data\\zone\\");
		FastFiles::AddZonePath("iw3sp_data\\zone\\english\\");
		FastFiles::AddZonePath("iw3sp_data\\zone\\french\\");
		FastFiles::AddZonePath("iw3sp_data\\zone\\german\\");
		FastFiles::AddZonePath("iw3sp_data\\zone\\italian\\");
		FastFiles::AddZonePath("iw3sp_data\\zone\\spanish\\");
		FastFiles::AddZonePath("iw3sp_data\\zone\\russian\\");

		Command::Add("loadzone", [](Command::Params* params)
		{
			if (params->size() < 2) return;

			Game::XZoneInfo info[2];

			// unload
			info[0].name = 0;
			info[0].allocFlags = Game::XZONE_FLAGS::XZONE_ZERO;
			info[0].freeFlags = Game::XZONE_FLAGS::XZONE_MOD;

			info[1].name = params->get(1);
			info[1].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			info[1].freeFlags = Game::XZONE_FLAGS::XZONE_ZERO;

			Game::DB_LoadXAssets(info, 2, true);
		});
	}

	FastFiles::~FastFiles()
	{
	}
}