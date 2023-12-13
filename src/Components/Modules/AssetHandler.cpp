#include "STDInc.hpp"

namespace Components
{
	thread_local int AssetHandler::BypassState = 0;
	bool AssetHandler::ShouldSearchTempAssets = false;
	std::map<Game::XAssetType, Utils::Slot<AssetHandler::Callback>> AssetHandler::TypeCallbacks;
	Utils::Signal<AssetHandler::RestrictCallback> AssetHandler::RestrictSignal;
	std::map<void*, void*> AssetHandler::Relocations;
	std::vector<std::pair<Game::XAssetType, std::string>> AssetHandler::EmptyAssets;
	std::map<std::string, Game::XAssetHeader> AssetHandler::TemporaryAssets[Game::XAssetType::ASSET_TYPE_COUNT];

	void AssetHandler::ClearTemporaryAssets()
	{
		for (int i = 0; i < Game::XAssetType::ASSET_TYPE_COUNT; ++i)
		{
			AssetHandler::TemporaryAssets[i].clear();
		}
	}

	void AssetHandler::StoreTemporaryAsset(Game::XAssetType type, Game::XAssetHeader asset)
	{
		AssetHandler::TemporaryAssets[type][Game::DB_GetXAssetNameHandlers[type](&asset)] = asset;
	}

	bool AssetHandler::IsAssetEligible(Game::XAssetType type, Game::XAssetHeader asset)
	{
		const char* name = Game::DB_GetXAssetNameHandlers[type](&asset);
		if (!name) return false;

		for (auto i = AssetHandler::EmptyAssets.begin(); i != AssetHandler::EmptyAssets.end();)
		{
			if (i->first == type && i->second == name)
			{
				i = AssetHandler::EmptyAssets.erase(i);
			}
			else
			{
				++i;
			}
		}

		bool restrict = false;
		AssetHandler::RestrictSignal(type, asset, name, &restrict);

		return (!restrict);
	}

	Game::XAssetHeader AssetHandler::FindTemporaryAsset(Game::XAssetType type, const char* filename)
	{
		Game::XAssetHeader header = { nullptr };
		if (type >= Game::XAssetType::ASSET_TYPE_COUNT) return header;

		auto tempPool = &AssetHandler::TemporaryAssets[type];
		auto entry = tempPool->find(filename);
		if (entry != tempPool->end())
		{
			header = { entry->second };
		}

		return header;
	}

	int AssetHandler::HasThreadBypass()
	{
		return AssetHandler::BypassState > 0;
	}

	void AssetHandler::SetBypassState(bool value)
	{
		AssetHandler::BypassState += (value ? 1 : -1);
		if (AssetHandler::BypassState < 0)
		{
			AssetHandler::BypassState = 0;
		}
	}

	void AssetHandler::ResetBypassState()
	{
		if (AssetHandler::HasThreadBypass())
		{
			AssetHandler::BypassState = 0;
		}
	}

	void AssetHandler::OnFind(Game::XAssetType type, Utils::Slot<AssetHandler::Callback> callback)
	{
		AssetHandler::TypeCallbacks[type] = callback;
	}

	void AssetHandler::OnLoad(Utils::Slot<AssetHandler::RestrictCallback> callback)
	{
		AssetHandler::RestrictSignal.connect(callback);
	}

	void AssetHandler::ClearRelocations()
	{
		AssetHandler::Relocations.clear();
	}

	void AssetHandler::Relocate(void* start, void* to, DWORD size)
	{
		for (DWORD i = 0; i < size; i += 4)
		{
			AssetHandler::Relocations[reinterpret_cast<char*>(start) + i] = reinterpret_cast<char*>(to) + i;
		}
	}

	Game::XAssetHeader AssetHandler::FindOriginalAsset(Game::XAssetType type, const char* filename)
	{
		AssetHandler::SetBypassState(true);
		Game::XAssetHeader header = Game::DB_FindXAssetHeader(type, filename);
		AssetHandler::SetBypassState(false);

		return header;
	}

	void AssetHandler::StoreEmptyAsset(Game::XAssetType type, const char* name)
	{
		AssetHandler::EmptyAssets.push_back({ type, name });
	}

	void AssetHandler::ExposeTemporaryAssets(bool expose)
	{
		AssetHandler::ShouldSearchTempAssets = expose;
	}

	Game::XAssetHeader AssetHandler::FindAssetStub(Game::XAssetType type, const char* name)
	{
		if (AssetHandler::HasThreadBypass() > 0)
		{
		checkTempAssets:
			if (AssetHandler::ShouldSearchTempAssets)
			{
				auto tempAsset = AssetHandler::FindTemporaryAsset(type, name);
				if (tempAsset.data != nullptr) return tempAsset;
				else goto finishOriginal;
			}
			else goto finishOriginal;
		}
		else
		{
			Game::XAssetHeader customAsset = { nullptr };

			if (name)
			{
				// Allow call DB_FindXAssetHeader within the hook
				AssetHandler::SetBypassState(true);

				if (AssetHandler::TypeCallbacks.find(type) != AssetHandler::TypeCallbacks.end())
				{
					customAsset = AssetHandler::TypeCallbacks[type](type, name);
				}

				// Disallow calling DB_FindXAssetHeader ;)
				AssetHandler::SetBypassState(false);

				if (customAsset.data != nullptr) return customAsset;
				else goto checkTempAssets;
			}
			else goto checkTempAssets;
		}

	finishOriginal:
		auto originalHeader = Game::DB_FindXAssetHeader(type, name); /*find_xasset_header_hook.invoke<Game::XAssetHeader>(type, name);*/

		return originalHeader;
	}

	void AssetHandler::reallocateEntryPool()
	{
		const size_t size = 789312;
		Game::XAssetEntry* entryPool = Utils::Memory::GetAllocator()->allocateArray<Game::XAssetEntry>(size);

		// Apply new size
		Utils::Hook::Set<DWORD>(0x45A6F0, size);

		// Apply new pool
		DWORD patches[] =
		{
			0x45A6E8, 0x45A918, 0x45A945, 0x45AA64, 0x45AAD5, 0x45AB28,
			0x45ABEE, 0x45B094, 0x45B144, 0x45B307, 0x45B3C3, 0x45B47A,
			0x45B4A3, 0x45B541, 0x45B696, 0x45C3ED, 0x45C544, 0x45C5C9,
			0x45C6EC, 0x45C764, 0x45CA24, 0x45CA78
		};

		for (int i = 0; i < ARRAYSIZE(patches); ++i)
		{
			Utils::Hook::Set<Game::XAssetEntry*>(patches[i], entryPool);
		}

		Utils::Hook::Set<Game::XAssetEntry*>(0x45A6D1, entryPool + 1);
		Utils::Hook::Set<Game::XAssetEntry*>(0x45A6E2, entryPool + 1);
	}


	AssetHandler::AssetHandler()
	{
		this->reallocateEntryPool();

		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_IMAGE, 7168);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_SOUND, 24000);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_LOADED_SOUND, 2700);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_FX, 1200);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_LOCALIZE_ENTRY, 14000);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_XANIMPARTS, 8192);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_XMODEL, 5125);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_PHYSPRESET, 128);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_MENU, 1280);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_MENULIST, 256);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_MATERIAL, 8192);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_WEAPON, 1024);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_STRINGTABLE, 800);
		Game::DB_ReallocXAssetPool(Game::ASSET_TYPE_GAMEWORLD_MP, 1);

		//AssetHandler::ClearTemporaryAssets();
		//Utils::Hook(Game::DB_FindXAssetHeader, AssetHandler::FindAssetStub).install()->quick();
	}

	AssetHandler::~AssetHandler()
	{
		AssetHandler::ClearTemporaryAssets();

		AssetHandler::Relocations.clear();
		AssetHandler::RestrictSignal.clear();
		AssetHandler::TypeCallbacks.clear();
	}
}