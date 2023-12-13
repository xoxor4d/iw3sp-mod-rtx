#pragma once

namespace Components
{
	class AssetHandler : public Component
	{
	public:
		typedef Game::XAssetHeader(Callback)(Game::XAssetType type, const std::string& name);
		typedef void(RestrictCallback)(Game::XAssetType type, Game::XAssetHeader asset, const std::string& name, bool* restrict);

		AssetHandler();
		~AssetHandler();

		static void OnFind(Game::XAssetType type, Utils::Slot<Callback> callback);
		static void OnLoad(Utils::Slot<RestrictCallback> callback);

		static void ClearRelocations();
		static void Relocate(void* start, void* to, DWORD size = 4);


		static Game::XAssetHeader FindOriginalAsset(Game::XAssetType type, const char* filename);

		static void ClearTemporaryAssets();
		static void StoreTemporaryAsset(Game::XAssetType type, Game::XAssetHeader asset);

		static void ResetBypassState();

		static void ExposeTemporaryAssets(bool expose);

	private:
		static thread_local int BypassState;
		static bool ShouldSearchTempAssets;

		static std::map<std::string, Game::XAssetHeader> TemporaryAssets[Game::XAssetType::ASSET_TYPE_COUNT];

		static std::map<Game::XAssetType, Utils::Slot<Callback>> TypeCallbacks;
		static Utils::Signal<RestrictCallback> RestrictSignal;

		static std::map<void*, void*> Relocations;

		static std::vector<std::pair<Game::XAssetType, std::string>> EmptyAssets;

		static Game::XAssetHeader FindTemporaryAsset(Game::XAssetType type, const char* filename);
		static bool IsAssetEligible(Game::XAssetType type, Game::XAssetHeader asset);
		static Game::XAssetHeader FindAssetStub(Game::XAssetType type, const char* name);

		static void StoreEmptyAsset(Game::XAssetType type, const char* name);

		static int HasThreadBypass();
		static void SetBypassState(bool value);

		void reallocateEntryPool();
	};
}